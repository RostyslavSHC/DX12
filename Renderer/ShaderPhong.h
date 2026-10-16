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
    void SetLightPosition(XMFLOAT3 _position); // Updates the light source position
    void SetSpecularStrength(float _strength); // Update the specular strength
    void SetSpecularColor(XMFLOAT4 _color); // Update the specular highlight color
    virtual void SetPipelineState() override;

protected:
    virtual void CreatePipelineState() override;
    virtual void CreateConstantBuffer(UINT64 _bufferSize) override;


private:
    // Members
    XMFLOAT3 m_lightPosition; // Stores the position of the light source
    float m_specularStrength; // Stores the current specular strength
    XMFLOAT4 m_specularColor; // Stores the current specular color
};
