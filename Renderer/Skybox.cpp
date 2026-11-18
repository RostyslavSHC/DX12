#include "Skybox.h"

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
}

void Skybox::CreateTexture(UINT16 _arraySize)
{
    Texture::CreateTexture(6);
}

void Skybox::CreateShaderResourceView(Shader* _shader,
    D3D12_SRV_DIMENSION _dimension)
{
    Texture::CreateShaderResourceView(_shader, D3D12_SRV_DIMENSION_TEXTURECUBE);
}
