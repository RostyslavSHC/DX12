#pragma once

#include "StandardIncludes.h"

struct DDS_PIXELFORMAT
{
	UINT size;
	UINT flags;
	UINT fourCC;
	UINT rgbBitCount;
	UINT rBitMask;
	UINT gBitMask;
	UINT bBitMask;
	UINT aBitMask;
};

struct DDS_HEADER
{
	UINT size;
	UINT flags;
	UINT height;
	UINT width;
	UINT pitchOrLinearSize;
	UINT depth;
	UINT mipMapCount;
	UINT reserved1[11];
	DDS_PIXELFORMAT ddsPixelFormat;
	UINT caps;
	UINT caps2;
	UINT caps3;
	UINT caps4;
	UINT reserved2;
};

class DDSLoad
{
public:
	// Constructors/Destructors
	DDSLoad();
	~DDSLoad();

	// Accessors
	DDS_HEADER* GetHeader() const { return m_header; }
	unsigned char* GetData() { return (m_data + m_offset); }
	DXGI_FORMAT GetFormat() const { return m_format; }
	UINT GetSize() const { return m_size; }

	// Methods
	void Load(wstring _filename);

private:
	// Methods
	void ReadDataFromFile(wstring _filename);
	DXGI_FORMAT ConvertFourCCToDXGI(uint32_t _fourcc);

	// Members
	static const UINT DDS_MAGIC = 0x20534444;
	DDS_HEADER* m_header;
	DXGI_FORMAT m_format;
	unsigned char* m_data;
	UINT m_offset;
	UINT m_size;
};