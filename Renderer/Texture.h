#pragma once

#include "StandardIncludes.h"

class Texture
{
public:
	// Constructors/Destructors
	Texture();
	virtual ~Texture() {}

	// Methods
	void Initialize(ID3D12DescriptorHeap* _descrHeap);
	void SetPipelineState(ID3D12DescriptorHeap* _descrHeap);

private:
	// Methods
	void CreateTexture();
	void CreateShaderResourceView(ID3D12DescriptorHeap* _descrHeap);
	void UploadTexture();
	std::vector<UINT8> GenerateTextureData();

	// Members
	ComPtr<ID3D12Resource> m_texture;
	UINT m_width;
	UINT m_height;
	UINT m_pixelSize;
};