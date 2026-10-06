#include "Texture.h"
#include "Renderer.h"

Texture::Texture()
{
	m_texture = nullptr;
	m_width = 256;
	m_height = 256;
	m_pixelSize = 4;
}

void Texture::Initialize(ID3D12DescriptorHeap* _descrHeap)
{
	CreateTexture();
	CreateShaderResourceView(_descrHeap);
	UploadTexture();
}

void Texture::CreateTexture()
{
	// Create the texture.
	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
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

void Texture::CreateShaderResourceView(ID3D12DescriptorHeap* _descrHeap)
{
	auto r = &Renderer::GetInstance();
	auto device = r->GetDevice().Get();
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(_descrHeap->GetCPUDescriptorHandleForHeapStart(), 1, descriptorSize);

	// Describe and create a SRV for the texture.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	std::vector<UINT8> texture = GenerateTextureData();
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = &texture[0];
	textureData.RowPitch = m_width * m_pixelSize;
	textureData.SlicePitch = textureData.RowPitch * m_height;

	auto c = r->GetCommands();
	c->Reset();
	UpdateSubresources(c->GetCommandList().Get(), m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
	c->AddBarrier(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	c->Close();
	c->Execute();
	r->WaitForPreviousFrame();
}

// Generate a simple black and white checkerboard texture.
std::vector<UINT8> Texture::GenerateTextureData()
{
	const UINT rowPitch = m_width * m_pixelSize;
	const UINT cellPitch = rowPitch >> 3;	// The width of a cell in the checkerboard texture.
	const UINT cellHeight = m_width >> 3;	// The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * m_height;

	std::vector<UINT8> data(textureSize);
	UINT8* pData = &data[0];

	for (UINT n = 0; n < textureSize; n += m_pixelSize)
	{
		UINT x = n % rowPitch;
		UINT y = n / rowPitch;
		UINT i = x / cellPitch;
		UINT j = y / cellHeight;

		if (i % 2 == j % 2)
		{
			pData[n] = 0x00;		// R
			pData[n + 1] = 0x00;	// G
			pData[n + 2] = 0x00;	// B
			pData[n + 3] = 0xff;	// A
		}
		else
		{
			pData[n] = 0xff;		// R
			pData[n + 1] = 0xff;	// G
			pData[n + 2] = 0xff;	// B
			pData[n + 3] = 0xff;	// A
		}
	}

	return data;
}

void Texture::SetPipelineState(ID3D12DescriptorHeap* _descrHeap)
{
	// Set SRV table (root parameter 1) - points to SRV in heap index 1
	auto r = &Renderer::GetInstance();
	UINT descriptorSize = r->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(_descrHeap->GetGPUDescriptorHandleForHeapStart(), 1, descriptorSize);
	r->GetCommands()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvHandle);
}