#include "ShaderDistanceTint.h"
#include "ITaggable.h"
#include "IRenderable.h"

ShaderDistanceTint::ShaderDistanceTint()
{
    SetShaderFile(L"DistanceTint.hlsl");
    m_playerPosition = { 0.0f, 0.0f, 0.0f };
}

ShaderDistanceTint::~ShaderDistanceTint()
{
}

void ShaderDistanceTint::CreatePipelineState()
{
    SetVertexType(&m_vertexType);
    SetInputElementDescs(m_vertexType.InputElementDescs);
    SetInputElemDescSize(_countof(m_vertexType.InputElementDescs));
    SetConstantBufferSize(sizeof(CB_DistanceTint));

    Shader::CreatePipelineState();
}

void ShaderDistanceTint::CreateConstantBuffer(UINT64 _bufferSize)
{
    Shader::CreateConstantBuffer(sizeof(CB_DistanceTint));
}

void ShaderDistanceTint::InitializeTexture(string _filename)
{
    m_texture.Initialize(_filename, this);
}

void ShaderDistanceTint::SetPlayerPosition(XMFLOAT3 _position)
{
    m_playerPosition = _position;
}

void ShaderDistanceTint::SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def)
{
    Shader::SetPipelineState(_renderable, _def);

    auto cbData = reinterpret_cast<CB_DistanceTint*>(ConstantBufferAddress());

    XMStoreFloat4(&cbData->PlayerPosition, FXMVECTOR{ m_playerPosition.x, m_playerPosition.y, m_playerPosition.z, 1.0f });

    // Check whether the renderable has been tagged
    ITaggable* taggable = dynamic_cast<ITaggable*>(_renderable);

    cbData->Tagged = 0.0f;

    if (taggable != nullptr && taggable->IsTagged())
    {
        cbData->Tagged = 1.0f;
    }
}