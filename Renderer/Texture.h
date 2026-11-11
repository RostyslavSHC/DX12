#pragma once

#include "StandardIncludes.h"

class DDSLoad;

class Texture
{
public:
    // Constructors/Destructors
    Texture();
    virtual ~Texture();

    // Methods
    void Initialize(string _filename, Shader* _shader);

private:
    // Methods
    void LoadTextureFromFile(wstring _filename);
    void CreateTexture();
    void CreateShaderResourceView(Shader* _shader);
    void UploadTexture();

    // Members
    DDSLoad* m_ddsLoader;
    ComPtr<ID3D12Resource> m_texture;
};
