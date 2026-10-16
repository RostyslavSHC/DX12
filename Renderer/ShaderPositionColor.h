#pragma once

#include "Shader.h"

class ShaderPositionColor : public Shader
{
public:
    // Constructors/Destructors
    ShaderPositionColor();

    // Methods
    virtual void SetPipelineState() override;

protected:
    // Methods
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;
};
