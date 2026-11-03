#pragma once

#include "Shader.h"

class ShaderPhong : public Shader
{
public:
    // Constructors/Destructors
    ShaderPhong();

    // Constant Buffer
    struct Material
    {
        XMFLOAT4 DiffuseColor;
        float SpecularStrength;
        XMFLOAT3 pad1;
        XMFLOAT4 SpecularColor;
    };

    struct Light
    {
        XMFLOAT4 AmbientColor;
        // XMFLOAT3 Position;
		XMFLOAT3 Direction;
        float pad0;
        XMFLOAT4 Color;
    };

    struct CB_Phong : ConstantBuffer
    {
        Material material;
        Light light;
        XMFLOAT4 CameraPosition;
    };


    // Methods
    virtual void SetPipelineState() override;

protected:
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;
};
