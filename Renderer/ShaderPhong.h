#pragma once

#include "Shader.h"

class ShaderPhong : public Shader
{
public:
    // Constructors/Destructors
    ShaderPhong();

    // Constant Buffer
    struct CB_Phong : ConstantBuffer
    {
        XMFLOAT4 DiffuseColor;
        XMFLOAT4 AmbientLight;
    };

    // Methods
    virtual void SetPipelineState() override;

protected:
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;
};
