#pragma once

#include "StandardIncludes.h"

class DDSLoad;

class Texture
{
public:
    // Constructors/Destructors
    Texture();
    virtual ~Texture();

    // Accessors
    int GetIndex() { return m_index; }

    // Methods
    void Initialize(string _filename, Shader* _shader);

protected:
    virtual void CreateTexture(UINT16 _arraySize = 1);
    virtual void CreateShaderResourceView(Shader* _shader, D3D12_SRV_DIMENSION _dimension = D3D12_SRV_DIMENSION_TEXTURE2D);

private:
    // Methods
    void LoadTextureFromFile(wstring _filename);
    void UploadTexture();

    // Members
    DDSLoad* m_ddsLoader;
    ComPtr<ID3D12Resource> m_texture;
    int m_index;
    UINT16 m_arraySize;
};