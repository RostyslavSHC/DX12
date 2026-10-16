#pragma once

#include "StandardIncludes.h"

class Texture;

class Mesh : public IRenderable
{
public:
	// Constructors/Destructors
	Mesh();
	virtual ~Mesh();

	// Methods
	void Initialize(Shader* _shader);
	void Initialize(Shader* _shader, wstring _textureFilename); // Initialize mesh with selected texture
	void Initialize(Shader* _shader, wstring _textureFilename, wstring _textureFilename2); // Initialize mesh with 2 selected textures

	virtual void Render(XMMATRIX _viewProj);

private:
	// Methods
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	// Members
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	UINT m_indexBufferSize;
	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};