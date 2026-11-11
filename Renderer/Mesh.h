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
    void Initialize(string _filename, Shader* _shader);
    virtual void Render(XMMATRIX _viewProj);

private:
    // Methods
    void CreateVertexBuffer(string _fileName);
    void CreateIndexBuffer();

    // Members
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

    UINT m_indexBufferSize;
    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};
