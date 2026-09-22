#include "Mesh.h"

Mesh::Mesh()
{
	m_vertexBuffer = nullptr;
	m_vertexBufferView = {};
}

void Mesh::Initialize(ID3D12Device* _device, Shader* _shader)
{
	SetShader(_shader);
	CreateVertexBuffer(_device);
}









void Mesh::CreateVertexBuffer(ID3D12Device* _device)
{
	Vertex triangleVertices[] =
	{
		{ { -0.5f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.25f, 0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

		{ { 0.0f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { 0.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.5f, 0.75f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
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




void Mesh::Render(ID3D12GraphicsCommandList* _commandList)
{
	_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	_commandList->DrawInstanced(6, 1, 0, 0);
}