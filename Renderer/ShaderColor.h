#pragma once

#include "Shader.h"

class ShaderColor : public Shader
{
public:
    // Constructors/Destructors
    ShaderColor();

    // Constant Buffer
    struct alignas(256) CB_Color : ConstantBuffer
    {
        XMFLOAT4 Color;
    };

    // Methods
    // virtual void SetPipelineState(IRenderable* _renderable) override;
    virtual void SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def = EN_PSD_DEFAULT) override;

protected:
    // Methods
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;
};
