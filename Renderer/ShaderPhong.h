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
        XMFLOAT3 LightPosition;
        float pad0; // Padding to align to 16 bytes
        XMFLOAT4 LightColor;
        float SpecularStrength;
        XMFLOAT3 pad1; // Padding to align to 16 bytes
        XMFLOAT4 SpecularColor;
        XMFLOAT4 CameraPosition;
    };

    // Methods
    virtual void SetPipelineState() override;

protected:
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;
};
