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
	void Initialize(wstring _filename, ID3D12DescriptorHeap* _descrHeap);
	void SetPipelineState(ID3D12DescriptorHeap* _descrHeap);

private:
	// Methods
	void LoadTextureFromFile(wstring _filename);
	void CreateTexture();
	void CreateShaderResourceView(ID3D12DescriptorHeap* _descrHeap);
	void UploadTexture();

	// Members
	DDSLoad* m_ddsLoader;
	ComPtr<ID3D12Resource> m_texture;
};