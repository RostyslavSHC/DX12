#pragma once

#include "StandardIncludes.h"

class Mesh : public IRenderable
{
public:
	// Constructors/Destructors
	Mesh();
	virtual ~Mesh() {}

	// Methods
	void Initialize(ID3D12Device* _device, Shader* _shader);
	virtual void Render(ID3D12GraphicsCommandList* _commandList);

private:
	// Methods
	void CreateVertexBuffer(ID3D12Device* _device);
	void CreateIndexBuffer(ID3D12Device* _device);

	// Members
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	UINT m_indexBufferSize;
	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};