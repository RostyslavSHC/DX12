#pragma once

#include "StandardIncludes.h"

class Texture;
class Vertex;

struct InstanceData
{
    XMFLOAT4X4 world;
    XMFLOAT4X4 invWorld;
};

class Mesh : public IRenderable
{
public:
    // Constructors/Destructors
    Mesh();
    virtual ~Mesh();

    // Methods
    void Initialize(string _filename, Shader* _shader, vector<InstanceData>* _instances = nullptr);
    virtual void Render(XMMATRIX _viewProj);

private:
    // Methods
    void CalculateTangents(vector<Vertex*>& _vertices, XMFLOAT3& _tangent, XMFLOAT3& _bitangent);
    void CreateVertexBuffer(string _fileName);
    void CreateInstanceBuffer(const vector<InstanceData>& _instances);
    void CreateIndexBuffer();

    // Members
    ComPtr<ID3D12Resource> m_vertexBuffer;
    ComPtr<ID3D12Resource> m_instanceBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_VERTEX_BUFFER_VIEW m_instanceBufferView;

    UINT m_indexBufferSize;
    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};
