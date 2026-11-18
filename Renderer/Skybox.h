#pragma once

#include "Texture.h"

class Skybox : public Texture
{
public:
    // Constructors/Destructors
    Skybox();
    virtual ~Skybox();

protected:
    // Methods
    virtual void CreateTexture(UINT16 _arraySize) override;
    virtual void CreateShaderResourceView(Shader* _shader,
        D3D12_SRV_DIMENSION _dimension) override;

private:
};
