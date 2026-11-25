#pragma once

#include "Shader.h"
#include "Texture.h"

class ShaderDistanceTint : public Shader
{
public:
    // Constructors/Destructors
    ShaderDistanceTint();
    virtual ~ShaderDistanceTint();

    // Constant Buffer
    struct alignas(256) CB_DistanceTint : ConstantBuffer
    {
        XMFLOAT4 PlayerPosition;
        float Tagged;
        float RenderSpotlights;
        float SpotlightCount;
        float Padding;

        XMFLOAT4 SpotlightPositions[9];
    };

    // Methods
    void InitializeTexture(string _filename);
    void SetPlayerPosition(XMFLOAT3 _position);

    virtual void SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def = EN_PSD_DEFAULT) override;

    void SetRenderSpotlights(bool _renderSpotlights);
    void SetSpotlightPositions(const vector<XMFLOAT3>& _positions);
    void SetBackground(IRenderable* _background);


protected:
    // Methods
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;

private:
    // Members
    VTX_PosNormTex m_vertexType;
    Texture m_texture;
    XMFLOAT3 m_playerPosition;

    bool m_renderSpotlights;
    vector<XMFLOAT3> m_spotlightPositions;
    IRenderable* m_background;


};