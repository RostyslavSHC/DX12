#include "Mesh.h"
#include "Renderer.h"
#include "SkyBox.h"
#include "ShaderPhong.h"
#include "ShaderSkybox.h"
#include "VertexStructs.h"
#include "OBJLoader.h"

Mesh::Mesh()
{
    m_vertexBuffer = nullptr;
    m_vertexBufferView = {};
    m_instanceBuffer = nullptr;
    m_instanceBufferView = {};
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

    t = GetTexNormal();
    if (t != nullptr)
    {
        delete t;
        SetTexNormal(nullptr);
    }
}


void Mesh::Initialize(string _filename, Shader* _shader, vector<InstanceData>* _instances)
{
    SetShader(_shader);
    CreateVertexBuffer(_filename);
    if (_instances != nullptr)
    {
        CreateInstanceBuffer(*_instances);
    }
    //CreateIndexBuffer();
}


// Slide 30 - Calculate Tangents and Bitangents
void Mesh::CalculateTangents(vector<Vertex*>& _vertices, XMFLOAT3& _tangent, XMFLOAT3& _bitangent)
{
    XMFLOAT3 xp0 = { _vertices[0]->pos.x, _vertices[0]->pos.y, _vertices[0]->pos.z };
    XMFLOAT3 xp1 = { _vertices[1]->pos.x, _vertices[1]->pos.y, _vertices[1]->pos.z };
    XMFLOAT3 xp2 = { _vertices[2]->pos.x, _vertices[2]->pos.y, _vertices[2]->pos.z };

    XMFLOAT2 xuv0 = { _vertices[0]->tex.u, _vertices[0]->tex.v };
    XMFLOAT2 xuv1 = { _vertices[1]->tex.u, _vertices[1]->tex.v };
    XMFLOAT2 xuv2 = { _vertices[2]->tex.u, _vertices[2]->tex.v };

    XMVECTOR p0 = XMLoadFloat3(&xp0);
    XMVECTOR p1 = XMLoadFloat3(&xp1);
    XMVECTOR p2 = XMLoadFloat3(&xp2);

    XMVECTOR uv0 = XMLoadFloat2(&xuv0);
    XMVECTOR uv1 = XMLoadFloat2(&xuv1);
    XMVECTOR uv2 = XMLoadFloat2(&xuv2);

    XMFLOAT3 edge1; XMStoreFloat3(&edge1, p1 - p0);
    XMFLOAT3 edge2; XMStoreFloat3(&edge2, p2 - p0);

    XMFLOAT2 deltaUV1; XMStoreFloat2(&deltaUV1, uv1 - uv0);
    XMFLOAT2 deltaUV2; XMStoreFloat2(&deltaUV2, uv2 - uv0);

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    _tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    _tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    _tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    _bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    _bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    _bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
}

void Mesh::CreateVertexBuffer(string _fileName)
{
    OBJLoader objLoader;
    objLoader.ReadFile(_fileName);
    objLoader.ProcessOBJ();

    if (objLoader.GetMapDiffuse() != "")
    {
        Texture* t0;
        if (dynamic_cast<ShaderSkybox*>(GetShader()) != nullptr)
        {
            t0 = new Skybox();
        }
        else
        {
            t0 = new Texture();
        }

        // Texture* t = new Texture();
        t0->Initialize("..\\Assets\\Textures\\" + objLoader.GetMapDiffuse(), GetShader());
        SetTexDiffuse(t0);
    }

    if (objLoader.GetMapSpecular() != "")
    {
        Texture* t = new Texture();
        t->Initialize("..\\Assets\\Textures\\" + objLoader.GetMapSpecular(), GetShader());
        SetTexSpecular(t);
    }

    if (objLoader.GetMapNormal() != "")
    {
        Texture* t2 = new Texture();
        t2->Initialize("..\\Assets\\Textures\\" + objLoader.GetMapNormal(), GetShader());
        SetTexNormal(t2);
    }

    VTX_Base* vtxb = GetShader()->GetVertexType();
    UINT vertexBufferSize = 0;
    UINT vertexBufferStride = 0;
    vector<float> meshData = {};

    switch (vtxb->GetVertexType())
    {
        // Slides 31-32 - Tangent-space vertex data
    case EN_VT_POSNRMTEXTANBIT:
    {
        vector<Vertex*> triangle;
        vector<XMFLOAT3> tangents;
        vector<XMFLOAT3> bitangents;
        XMFLOAT3 tangent;
        XMFLOAT3 bitangent;

        auto& vs = objLoader.GetVertices();

        for (unsigned int j = 0; j < vs.size(); j += 3)
        {
            triangle.clear();

            for (unsigned int vc = j; vc < j + 3; vc++)
            {
                triangle.push_back(&vs[vc]);
            }

            CalculateTangents(triangle, tangent, bitangent);
            tangents.push_back(tangent);
            bitangents.push_back(bitangent);
        }

        for (unsigned int j = 0; j < vs.size(); j++)
        {
            auto& v = vs[j];

            meshData.push_back(v.pos.x);
            meshData.push_back(v.pos.y);
            meshData.push_back(v.pos.z);

            meshData.push_back(v.nrm.nx);
            meshData.push_back(v.nrm.ny);
            meshData.push_back(v.nrm.nz);

            meshData.push_back(v.tex.u);
            meshData.push_back(v.tex.v);


            int index = j / 3;
            meshData.push_back(tangents[index].x);
            meshData.push_back(tangents[index].y);
            meshData.push_back(tangents[index].z);
            meshData.push_back(bitangents[index].x);
            meshData.push_back(bitangents[index].y);
            meshData.push_back(bitangents[index].z);
        }

        vertexBufferStride = 14 * sizeof(float);
        break;
    }

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

    M_ASSERT(SUCCEEDED(device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDescr,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer))),
        "Failed to create commit resource.");

    // Copy the triangle data to the vertex buffer.
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);

    M_ASSERT(SUCCEEDED(m_vertexBuffer->Map(
        0,
        &readRange,
        reinterpret_cast<void**>(&pVertexDataBegin))),
        "Failed to map vertex buffer");

    memcpy(pVertexDataBegin, meshData.data(), vertexBufferSize);
    m_vertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = vertexBufferStride;
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
}











void Mesh::CreateInstanceBuffer(const vector<InstanceData>& _instances)
{
    UINT instanceBufferSize = static_cast<UINT>(_instances.size() * sizeof(InstanceData));
    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(instanceBufferSize);
    auto device = Renderer::GetInstance().GetDevice().Get();

    M_ASSERT(SUCCEEDED(device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&m_instanceBuffer))),
        "Failed to create Instance Buffer");

    // Copy the instance data to the instance buffer.
    UINT8* pInstanceDataBegin;
    CD3DX12_RANGE readRange(0, 0);

    M_ASSERT(SUCCEEDED(m_instanceBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pInstanceDataBegin))),
        "Failed to map instance buffer");

    memcpy(pInstanceDataBegin, _instances.data(), instanceBufferSize);
    m_instanceBuffer->Unmap(0, nullptr);

    // Initialize the instance buffer view.
    m_instanceBufferView.BufferLocation = m_instanceBuffer->GetGPUVirtualAddress();
    m_instanceBufferView.StrideInBytes = sizeof(InstanceData);
    m_instanceBufferView.SizeInBytes = instanceBufferSize;
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
        &hProp,
        D3D12_HEAP_FLAG_NONE,
        &rDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_indexBuffer))),
        "Failed to create index buffer commit resource");

    UINT8* pIndexDataBegin;
    CD3DX12_RANGE readRange(0, 0);

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
    auto r = &Renderer::GetInstance();
    auto c = r->GetCommands()->GetCommandList().Get();
    auto cbData = reinterpret_cast<Shader::ConstantBuffer*>(GetShader()->ConstantBufferAddress());
    XMStoreFloat4x4(&cbData->WVP, XMMatrixTranspose(_viewProj));
    c->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    if (m_instanceBuffer != nullptr)
    {
        D3D12_VERTEX_BUFFER_VIEW views[2] = { m_vertexBufferView, m_instanceBufferView };
        c->IASetVertexBuffers(0, 2, views);
        UINT numVertices = m_vertexBufferView.SizeInBytes / m_vertexBufferView.StrideInBytes;
        UINT numInstances = m_instanceBufferView.SizeInBytes / m_instanceBufferView.StrideInBytes;
        c->DrawInstanced(numVertices, numInstances, 0, 0);
    }
    else
    {
        c->IASetVertexBuffers(0, 1, &m_vertexBufferView);
        UINT numVertices = m_vertexBufferView.SizeInBytes / m_vertexBufferView.StrideInBytes;
        c->DrawInstanced(numVertices, 1, 0, 0);
    }
}


