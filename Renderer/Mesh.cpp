#include "Mesh.h"
#include "Renderer.h"
#include "ShaderPhong.h"
#include "VertexStructs.h"
#include "OBJLoader.h"

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

    t = GetTexSpecular();
    if (t != nullptr)
    {
        delete t;
        SetTexSpecular(nullptr);
    }
}

void Mesh::Initialize(string _filename, Shader* _shader)
{
    SetShader(_shader);
    CreateVertexBuffer(_filename);
    //CreateIndexBuffer();
}

void Mesh::CreateVertexBuffer(string _fileName)
{
    OBJLoader objLoader;
    objLoader.ReadFile("..\\Assets\\Models\\teapot.obj");
    objLoader.ProcessOBJ();

    if (objLoader.GetMapDiffuse() != "")
    {
        Texture* t = new Texture();
        t->Initialize("..\\Assets\\Textures\\" + objLoader.GetMapDiffuse(), GetShader());
        SetTexDiffuse(t);
    }

    if (objLoader.GetMapSpecular() != "")
    {
        Texture* t = new Texture();
        t->Initialize("..\\Assets\\Textures\\" + objLoader.GetMapSpecular(), GetShader());
        SetTexSpecular(t);
    }

    VTX_Base* vtxb = GetShader()->GetVertexType();
    UINT vertexBufferSize = 0;
    UINT vertexBufferStride = 0;
    vector<float> meshData = {};

    switch (vtxb->GetVertexType())
    {
    case EN_VT_POSNRMTEX:
        for (auto& v : objLoader.GetVertices())
        {
            meshData.push_back(v.pos.x);
            meshData.push_back(v.pos.y);
            meshData.push_back(v.pos.z);
            meshData.push_back(v.nrm.nx);
            meshData.push_back(v.nrm.ny);
            meshData.push_back(v.nrm.nz);
            meshData.push_back(v.tex.u);
            meshData.push_back(v.tex.v);
        }
        vertexBufferStride = 8 * sizeof(float);
        break;

    case EN_VT_POS:
        for (auto& v : objLoader.GetVertices())
        {
            meshData.push_back(v.pos.x);
            meshData.push_back(v.pos.y);
            meshData.push_back(v.pos.z);
        }
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
    // GetShader()->SetPipelineState();

    auto r = &Renderer::GetInstance();
    auto c = r->GetCommands()->GetCommandList().Get();
    auto cbData = reinterpret_cast<Shader::ConstantBuffer*>(GetShader()->ConstantBufferAddress());

    // Shader::ConstantBuffer* cbData = &GetShader()->GetConstantData()[r->GetFrameIndex()];
    XMStoreFloat4x4(&cbData->WVP, XMMatrixTranspose(_viewProj));

    c->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    c->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    c->IASetIndexBuffer(&m_indexBufferView);
    // c->DrawIndexedInstanced(m_indexBufferSize / sizeof(unsigned short), 1, 0, 0, 0);
    c->DrawInstanced(m_vertexBufferView.SizeInBytes / m_vertexBufferView.StrideInBytes, 1, 0, 0);
}
