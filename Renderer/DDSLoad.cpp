#include "DDSLoad.h"

DDSLoad::DDSLoad()
{
	m_header = nullptr;
	m_data = nullptr;
	m_offset = 0;
	m_size = 0;
	m_format = DXGI_FORMAT_UNKNOWN;
}

DDSLoad::~DDSLoad()
{
	if (m_data != nullptr)
	{
		free(m_data);
		m_data = nullptr;
	}
}

void DDSLoad::ReadDataFromFile(wstring _filename)
{
	FILE_STANDARD_INFO fileInfo = {};
	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;
	Wrappers::FileHandle file(CreateFile2(_filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING,
		&extendedParams));

	M_ASSERT(file.Get() != INVALID_HANDLE_VALUE, "Failed to create DDS file handle.");
	M_ASSERT(GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)),
		"Get DDS file information failed.");
	M_ASSERT(fileInfo.EndOfFile.HighPart == 0, "Corrupt DDS file.");
	m_data = reinterpret_cast<unsigned char*>(malloc(fileInfo.EndOfFile.LowPart));
	m_size = fileInfo.EndOfFile.LowPart;
	M_ASSERT(ReadFile(file.Get(), m_data, fileInfo.EndOfFile.LowPart, nullptr, nullptr), "Reading DDS file failed.");
}

void DDSLoad::Load(wstring _filename)
{
	ReadDataFromFile(_filename);

	UINT magicNumber = *reinterpret_cast<const UINT*>(m_data);
	M_ASSERT(magicNumber == 0x20534444, "DDS header corrupt or not a DDS file."); // "DDS "
	m_header = reinterpret_cast<DDS_HEADER*>(m_data + sizeof(UINT));

	// Initial offset for standard DDS
	m_offset = sizeof(UINT) + sizeof(DDS_HEADER);
	if (m_header->ddsPixelFormat.fourCC == MAKEFOURCC('D', 'X', '1', '0'))
	{
		// Cast the memory immediately following the standard header
		auto dx10Header = reinterpret_cast<DDS_HEADER_DXT10*>(m_data + m_offset);
		m_format = dx10Header->dxgiFormat;
		// Move the data offset forward to account for the DX10 header
		m_offset += sizeof(DDS_HEADER_DXT10);
	}
	else
	{
		m_format = ConvertFourCCToDXGI(m_header->ddsPixelFormat.fourCC);
	}
	// Correct the remaining size of the actual pixel data
	m_size = m_size - m_offset;
}

DXGI_FORMAT DDSLoad::ConvertFourCCToDXGI(uint32_t _fourcc)
{
	// 1. Handle Compressed Formats via FourCC
	switch (_fourcc)
	{
	case MAKEFOURCC('D', 'X', 'T', '1'): return DXGI_FORMAT_BC1_UNORM;
	case MAKEFOURCC('D', 'X', 'T', '2'): // DXT2 is DXT3 with premultiplied alpha
	case MAKEFOURCC('D', 'X', 'T', '3'): return DXGI_FORMAT_BC2_UNORM;
	case MAKEFOURCC('D', 'X', 'T', '4'): // DXT4 is DXT5 with premultiplied alpha
	case MAKEFOURCC('D', 'X', 'T', '5'): return DXGI_FORMAT_BC3_UNORM;

	case MAKEFOURCC('A', 'T', 'I', '1'):
	case MAKEFOURCC('B', 'C', '4', 'U'): return DXGI_FORMAT_BC4_UNORM;
	case MAKEFOURCC('B', 'C', '4', 'S'): return DXGI_FORMAT_BC4_SNORM;

	case MAKEFOURCC('A', 'T', 'I', '2'):
	case MAKEFOURCC('B', 'C', '5', 'U'): return DXGI_FORMAT_BC5_UNORM;
	case MAKEFOURCC('B', 'C', '5', 'S'): return DXGI_FORMAT_BC5_SNORM;

	case MAKEFOURCC('R', 'G', 'B', 'G'): return DXGI_FORMAT_R8G8_B8G8_UNORM;
	case MAKEFOURCC('G', 'R', 'G', 'B'): return DXGI_FORMAT_G8R8_G8B8_UNORM;

		// If FourCC is "DX10", the real format is in the DDS_HEADER_DXT10 extension
	case MAKEFOURCC('D', 'X', '1', '0'): return DXGI_FORMAT_UNKNOWN;

		// 2. Handle Legacy Uncompressed RGB Formats
		// If fourCC is 0, we must check the bitmask to determine the format
	case 0:
	{
		// Check for common RGBA8 (32-bit)
		if (m_header->ddsPixelFormat.rgbBitCount == 32)
		{
			if (m_header->ddsPixelFormat.rBitMask == 0x00ff0000) return DXGI_FORMAT_B8G8R8A8_UNORM;
			if (m_header->ddsPixelFormat.rBitMask == 0x000000ff) return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		break;
	}
	}

	return DXGI_FORMAT_UNKNOWN;
}
