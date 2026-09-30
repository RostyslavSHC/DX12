#include "Mesh.h"

Mesh::Mesh()
{
	m_vertexBuffer = nullptr;
	m_vertexBufferView = {};
	m_indexBufferSize = 0;
	m_indexBuffer = nullptr;
	m_indexBufferView = {};
}

void Mesh::Initialize(ID3D12Device* _device, Shader* _shader)
{
	SetShader(_shader);
	CreateVertexBuffer(_device);
	CreateIndexBuffer(_device);
}









void Mesh::CreateVertexBuffer(ID3D12Device* _device)
{
	Vertex triangleVertices[] =
	{
		/* Position */					/* RGBA Color */
		{{ 0.00f,  0.00f, 0.00f}, {1.0f,   0.0f,   0.0f,   1.0f}}, // Red
		{{-0.50f,  0.00f, 0.00f}, {0.0f,   1.0f,   0.0f,   1.0f}}, // Green
		{{-0.25f,  0.50f, 0.00f}, {0.0f,   0.0f,   1.0f,   1.0f}}, // Blue
		{{ 0.25f,  0.50f, 0.00f}, {1.0f,   1.0f,   1.0f,   1.0f}}, // White
		{{ 0.50f,  0.00f, 0.00f}, {1.0f,   1.0f,   0.0f,   1.0f}}, // Yellow
		{{ 0.25f, -0.50f, 0.00f}, {0.294f, 0.0f,   0.51f,  1.0f}}, // Indigo
		{{-0.25f, -0.50f, 0.00f}, {0.502f, 0.0f,   0.502f, 1.0f}}  // Purple
	};

	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDescr = CD3DX12_RESOURCE_DESC::Buffer(sizeof(triangleVertices));
	M_ASSERT(SUCCEEDED(_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDescr,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&m_vertexBuffer))),
		"Failed to create commit resource.");

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
	M_ASSERT(SUCCEEDED(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin))),
		"Failed to map vertex buffer");
	memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
	m_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_vertexBufferView.SizeInBytes = sizeof(triangleVertices);
}




void Mesh::CreateIndexBuffer(ID3D12Device* _device)
{
	unsigned short iList[] = {
		0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,1
	};

	m_indexBufferSize = sizeof(iList);

	auto hProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto rDesc = CD3DX12_RESOURCE_DESC::Buffer(m_indexBufferSize);

	M_ASSERT(SUCCEEDED(_device->CreateCommittedResource(
		&hProp, // Use upload heap
		D3D12_HEAP_FLAG_NONE,
		&rDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // Start in a state the GPU can read from
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer))), "Failed to create index buffer commit resource");

	UINT8* pIndexDataBegin;
	CD3DX12_RANGE readRange(0, 0); // No intention to allow CPU read rights, so read range is (0, 0).
	M_ASSERT(SUCCEEDED(m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin))), "Failed to map index buffer upload heap");

	memcpy(pIndexDataBegin, iList, m_indexBufferSize);
	m_indexBuffer->Unmap(0, nullptr);

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = m_indexBufferSize;
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
}


























void Mesh::Render(ID3D12GraphicsCommandList* _commandList, XMMATRIX _viewProj)
{
	ConstantBuffer* cbData = &GetShader()->GetConstantData()[Renderer::GetInstance().GetFrameIndex()];
	XMStoreFloat4x4(&cbData->WVP, XMMatrixTranspose(_viewProj));

	_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	_commandList->IASetIndexBuffer(&m_indexBufferView);
	//_commandList->DrawInstanced(9, 1, 0, 0);
	_commandList->DrawIndexedInstanced(m_indexBufferSize, 1, 0, 0, 0);
}