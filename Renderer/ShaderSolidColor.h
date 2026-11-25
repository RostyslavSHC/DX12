#pragma once

#include "ShaderColor.h"

class ShaderSolidColor : public ShaderColor
{
public:
    // Constructors/Destructors
    ShaderSolidColor();

    // Methods
    void SetColor(XMFLOAT4 _color);
    virtual void SetPipelineState(IRenderable* _renderable,
        PipelineStateDefs _def = EN_PSD_DEFAULT) override;

private:
    // Members
    XMFLOAT4 m_color;
};