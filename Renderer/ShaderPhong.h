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
        uint32_t TexIndDiffuse;
        uint32_t TexIndSpecular;
        float pad0;
        XMFLOAT4 SpecularColor;
    };

    struct Light
    {
        XMFLOAT4 AmbientColor;
        XMFLOAT3 Position;
		// XMFLOAT3 Direction;
        float pad0;
        XMFLOAT4 Color;
		XMFLOAT3 ConstLinQuad; // Pack 3 values: constant, linear, quadratic
		float pad1;
        XMFLOAT3 SpotDirection;
		float pad2;
        XMFLOAT2 SpotConeFalloff; // Pack 2 values: inner cone, outer cone
		XMFLOAT2 pad3;
    };

    struct CB_Phong : ConstantBuffer
    {
        Material material;
        Light lights[8];
        UINT LightCount;
        XMFLOAT3 pad0;
        XMFLOAT4 CameraPosition;
    };


    // Methods
    // virtual void SetPipelineState(IRenderable* _renderable) override;
    virtual void SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def = EN_PSD_DEFAULT) override;

protected:
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;
};
