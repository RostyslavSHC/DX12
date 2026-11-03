#include "Mesh.h"
#include "Renderer.h"
#include "ShaderPhong.h"
#include "VertexStructs.h"

Mesh::Mesh()
{
	m_vertexBuffer = nullptr;
	m_vertexBufferView = {};
	m_indexBufferSize = 0;
	m_indexBuffer = nullptr;
	m_indexBufferView = {};
}

Mesh::~Mesh()
{
	Texture* t = GetTexDiffuse();
	if (t != nullptr)
	{
		delete t;
		SetTexDiffuse(nullptr);
	}
	t = GetTexture1();
	if (t != nullptr)
	{
		delete t;
		SetTexture1(nullptr);
	}
}

void Mesh::Initialize(Shader* _shader)
{
	SetShader(_shader);
	CreateVertexBuffer();
	//CreateIndexBuffer();
	Texture* t0 = new Texture();
	t0->Initialize(L"..\\Assets\\Textures\\MetalFrameWood.dds", _shader);
	SetTexDiffuse(t0);

	Texture* t1 = new Texture();
	t1->Initialize(L"..\\Assets\\Textures\\MetalFrame.dds", _shader);
	SetTexture1(t1);
}

void Mesh::CreateVertexBuffer()
{
	VTX_Base* vtxb = GetShader()->GetVertexType();
	UINT vertexBufferSize = 0;
	UINT vertexBufferStride = 0;
	vector<float> meshData = {};

	switch (vtxb->GetVertexType())
	{
	case EN_VT_POSNRMTEX:
		meshData =
		{
			/* POSITION */        /*NORMAL*/ /* TEXCOORDS */
			-0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 1, // Front Face
			-0.5f,  0.5f, -0.5f, 0, 0, -1, 0, 0,
			 0.5f,  0.5f, -0.5f, 0, 0, -1, 1, 0,
			-0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 1,
			 0.5f,  0.5f, -0.5f, 0, 0, -1, 1, 0,
			 0.5f, -0.5f, -0.5f, 0, 0, -1, 1, 1,

			-0.5f, -0.5f,  0.5f, 0, 0, 1, 1, 1, // Back Face
			 0.5f, -0.5f,  0.5f, 0, 0, 1, 0, 1,
			 0.5f,  0.5f,  0.5f, 0, 0, 1, 0, 0,
			-0.5f, -0.5f,  0.5f, 0, 0, 1, 1, 1,
			 0.5f,  0.5f,  0.5f, 0, 0, 1, 0, 0,
			-0.5f,  0.5f,  0.5f, 0, 0, 1, 1, 0,

			-0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1, // Top Face
			-0.5f, 0.5f,  0.5f, 0, 1, 0, 0, 0,
			 0.5f, 0.5f,  0.5f, 0, 1, 0, 1, 0,
			-0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1,
			 0.5f, 0.5f,  0.5f, 0, 1, 0, 1, 0,
			 0.5f, 0.5f, -0.5f, 0, 1, 0, 1, 1,

			-0.5f, -0.5f, -0.5f, 0, -1, 0, 1, 1, // Bottom Face
			 0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 1,
			 0.5f, -0.5f,  0.5f, 0, -1, 0, 0, 0,
			-0.5f, -0.5f, -0.5f, 0, -1, 0, 1, 1,
			 0.5f, -0.5f,  0.5f, 0, -1, 0, 0, 0,
			-0.5f, -0.5f,  0.5f, 0, -1, 0, 1, 0,

			 0.5f, -0.5f, -0.5f, 1, 0, 0, 0, 1, // Right Face
			 0.5f,  0.5f, -0.5f, 1, 0, 0, 0, 0,
			 0.5f,  0.5f,  0.5f, 1, 0, 0, 1, 0,
			 0.5f, -0.5f, -0.5f, 1, 0, 0, 0, 1,
			 0.5f,  0.5f,  0.5f, 1, 0, 0, 1, 0,
			 0.5f, -0.5f,  0.5f, 1, 0, 0, 1, 1,

			-0.5f, -0.5f, -0.5f, -1, 0, 0, 1, 1, // Left Face
			-0.5f, -0.5f,  0.5f, -1, 0, 0, 0, 1,
			-0.5f,  0.5f,  0.5f, -1, 0, 0, 0, 0,
			-0.5f, -0.5f, -0.5f, -1, 0, 0, 1, 1,
			-0.5f,  0.5f,  0.5f, -1, 0, 0, 0, 0,
			-0.5f,  0.5f, -0.5f, -1, 0, 0, 1, 0
		};
		vertexBufferStride = 8 * sizeof(float);
		break;

	case EN_VT_POS:
		meshData =
		{
			// POSITION
			-0.5f, -0.5f, -0.5f, // Front Face
			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f, // Back Face
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			-0.5f, 0.5f, -0.5f, // Top Face
			-0.5f, 0.5f,  0.5f,
			 0.5f, 0.5f,  0.5f,
			-0.5f, 0.5f, -0.5f,
			 0.5f, 0.5f,  0.5f,
			 0.5f, 0.5f, -0.5f,

			-0.5f, -0.5f, -0.5f, // Bottom Face
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			 0.5f, -0.5f, -0.5f, // Right Face
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f, // Left Face
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f
		};
		vertexBufferStride = 3 * sizeof(float);
		break;
	}

	vertexBufferSize = (UINT)meshData.size() * sizeof(float);
	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDescr = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	auto device = Renderer::GetInstance().GetDevice().Get();
	M_ASSERT(SUCCEEDED(device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDescr,
														D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
														IID_PPV_ARGS(&m_vertexBuffer))),
														"Failed to create commit resource.");

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
	M_ASSERT(SUCCEEDED(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))),
		"Failed to map vertex buffer");
	memcpy(pVertexDataBegin, meshData.data(), vertexBufferSize);
	m_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = vertexBufferStride;
	m_vertexBufferView.SizeInBytes = vertexBufferSize;
}

void Mesh::CreateIndexBuffer()
{
	unsigned short iList[] = {
		0,1,2,0,3,1
	};

	m_indexBufferSize = sizeof(iList);

	auto hProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto rDesc = CD3DX12_RESOURCE_DESC::Buffer(m_indexBufferSize);
	auto device = Renderer::GetInstance().GetDevice().Get();

	M_ASSERT(SUCCEEDED(device->CreateCommittedResource(
		&hProp, // Use upload heap
		D3D12_HEAP_FLAG_NONE,
		&rDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // Start in a state the GPU can read from
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer))),
		"Failed to create index buffer commit resource");

	UINT8* pIndexDataBegin;
	CD3DX12_RANGE readRange(0, 0); // No intention to allow CPU read rights, so read range is (0, 0).
	M_ASSERT(SUCCEEDED(m_indexBuffer->Map(
		0,
		&readRange,
		reinterpret_cast<void**>(&pIndexDataBegin))),
		"Failed to map index buffer upload heap");

	memcpy(pIndexDataBegin, iList, m_indexBufferSize);
	m_indexBuffer->Unmap(0, nullptr);

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = m_indexBufferSize;
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void Mesh::Render(XMMATRIX _viewProj)
{
	GetShader()->SetPipelineState();

	auto r = &Renderer::GetInstance();
	auto c = r->GetCommands()->GetCommandList().Get();

	Shader::ConstantBuffer* cbData = &GetShader()->GetConstantData()[r->GetFrameIndex()];
	XMStoreFloat4x4(&cbData->WVP, XMMatrixTranspose(_viewProj));

	c->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	c->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	c->IASetIndexBuffer(&m_indexBufferView);
	// c->DrawIndexedInstanced(m_indexBufferSize / sizeof(unsigned short), 1, 0, 0, 0);
	c->DrawInstanced(36, 1, 0, 0);
}
