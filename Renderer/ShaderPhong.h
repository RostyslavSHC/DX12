
#pragma once

#include "Shader.h"

class ShaderPhong : public Shader
{
public:
    // Constructors/Destructors
    ShaderPhong();

protected:
    void CreatePipelineState() override;
};
