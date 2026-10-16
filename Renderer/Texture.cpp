#include "Texture.h"
#include "Renderer.h"
#include "DDSLoad.h"
#include "Shader.h"

Texture::Texture()
{
	m_texture = nullptr;
	m_ddsLoader = nullptr;
}

Texture::~Texture()
{
	if (m_ddsLoader != nullptr)
	{
		delete m_ddsLoader;
		m_ddsLoader = nullptr;
	}
}

void Texture::Initialize(wstring _filename, Shader* _shader)
{
	LoadTextureFromFile(_filename);
	CreateTexture();
	CreateShaderResourceView(_shader);
	UploadTexture();
}

void Texture::LoadTextureFromFile(wstring _filename)
{
	m_ddsLoader = new DDSLoad();
	m_ddsLoader->Load(_filename);
}

void Texture::CreateTexture()
{
	// Create the texture.
	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.MipLevels = 1;
	textureDesc.Format = m_ddsLoader->GetFormat();
	textureDesc.Width = m_ddsLoader->GetHeader()->width;
	textureDesc.Height = m_ddsLoader->GetHeader()->height;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto device = Renderer::GetInstance().GetDevice().Get();
	M_ASSERT(SUCCEEDED(device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_texture))),
		"Failed to create committed texture resource.");
}

void Texture::CreateShaderResourceView(Shader* _shader)
{
	auto r = &Renderer::GetInstance();
	auto device = r->GetDevice().Get();
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(_shader->GetDescrHeap()->GetCPUDescriptorHandleForHeapStart(), _shader->NextSRVIndex(), descriptorSize);

	// Describe and create a SRV for the texture.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_ddsLoader->GetFormat();
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(m_texture.Get(), &srvDesc, srvHandle);
}

void Texture::UploadTexture()
{
	// Create the GPU upload buffer.
	ComPtr<ID3D12Resource> textureUploadHeap;
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resDescr = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
	auto r = &Renderer::GetInstance();
	auto device = r->GetDevice().Get();
	auto res = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resDescr,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&textureUploadHeap));
	M_ASSERT(SUCCEEDED(res), "Failed to create texture upload heap.");

	// Copy data to the intermediate upload heap and then schedule a copy from the upload heap to the Texture2D.
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = m_ddsLoader->GetData();


	// DXT1 compresses a group of 4 × 4 pixels into an 8-byte block
	// My original code callculates number of bytes in each row of blocks correctly for textures with widths divisible by 4,
	// but then multiplies by the full image height instead of the number of compressed block rows
	
	int blocksPerRow = m_ddsLoader->GetHeader()->width / 4;
	textureData.RowPitch = blocksPerRow * 8;
	textureData.SlicePitch = textureData.RowPitch * m_ddsLoader->GetHeader()->height;
	

	/*
	UINT width = m_ddsLoader->GetHeader()->width;
	UINT height = m_ddsLoader->GetHeader()->height;

	// DXT1 textures store pixels in 4x4 blocks, using 8 bytes per block
	UINT blocksPerRow = (width + 3) / 4;
	UINT blockRows = (height + 3) / 4;

	textureData.RowPitch = blocksPerRow * 8;
	textureData.SlicePitch = textureData.RowPitch * blockRows;
	*/





	auto c = r->GetCommands();
	c->Reset();
	UpdateSubresources(c->GetCommandList().Get(), m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
	c->AddBarrier(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	c->Close();
	c->Execute();
	r->WaitForPreviousFrame();
}
