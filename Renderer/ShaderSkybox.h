#pragma once

#include "Shader.h"

class ShaderSkybox : public Shader
{
public:
    // Constructors/Destructors
    ShaderSkybox();

    // Constant Buffer
    struct alignas(256) CB_Skybox : ConstantBuffer
    {
        XMFLOAT4X4 View;
        XMFLOAT4X4 Projection;
    };

    // Methods
    virtual void SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def = EN_PSD_DEFAULT) override;

protected:
    // Methods
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;
};
