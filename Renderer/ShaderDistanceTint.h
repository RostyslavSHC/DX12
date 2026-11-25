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
    };

    // Methods
    void InitializeTexture(string _filename);
    void SetPlayerPosition(XMFLOAT3 _position);

    virtual void SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def = EN_PSD_DEFAULT) override;

protected:
    // Methods
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;

private:
    // Members
    VTX_PosNormTex m_vertexType;
    Texture m_texture;
    XMFLOAT3 m_playerPosition;
};