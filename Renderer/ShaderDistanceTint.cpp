#include "ShaderDistanceTint.h"
#include "ITaggable.h"
#include "IRenderable.h"

ShaderDistanceTint::ShaderDistanceTint()
{
    SetShaderFile(L"DistanceTint.hlsl");
    m_playerPosition = { 0.0f, 0.0f, 0.0f };
    m_renderSpotlights = false;
    m_background = nullptr;
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


void ShaderDistanceTint::SetRenderSpotlights(bool _renderSpotlights)
{
    m_renderSpotlights = _renderSpotlights;
}

void ShaderDistanceTint::SetSpotlightPositions(const vector<XMFLOAT3>& _positions)
{
    m_spotlightPositions = _positions;
}

void ShaderDistanceTint::SetBackground(IRenderable* _background)
{
    m_background = _background;
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


    // Set the spotlight rendering data
    // cbData->RenderSpotlights = m_renderSpotlights ? 1.0f : 0.0f;

    // Apply spotlights to the background only
    cbData->RenderSpotlights = (m_renderSpotlights && _renderable == m_background) ? 1.0f : 0.0f;



    int spotlightCount = (int)m_spotlightPositions.size();

    if (spotlightCount > 9)
        spotlightCount = 9;

    cbData->SpotlightCount = (float)spotlightCount;
    cbData->Padding = 0.0f;

    // Set the spotlight positions
    for (int i = 0; i < spotlightCount; i++)
    {
        XMFLOAT3 position = m_spotlightPositions[i];

        cbData->SpotlightPositions[i] = { position.x, position.y, position.z, 1.0f };
    }
}