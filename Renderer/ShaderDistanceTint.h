#pragma once

#include "Shader.h"
#include "Texture.h"

class ShaderDistanceTint : public Shader
{
public:
    // Constructors/Destructors
    ShaderDistanceTint();
    virtual ~ShaderDistanceTint();

    // Methods
    void InitializeTexture(string _filename);

protected:
    // Methods
    virtual void CreatePipelineState() override;

private:
    // Members
    VTX_PosNormTex m_vertexType;
    Texture m_texture;
};