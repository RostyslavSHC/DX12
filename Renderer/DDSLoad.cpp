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

	// DDS files always start with the same magic number.
	UINT magicNumber = *reinterpret_cast<const UINT*>(m_data);
	M_ASSERT(magicNumber == DDS_MAGIC, "DDS header corrupt or not a DDS file.");

	m_header = reinterpret_cast<DDS_HEADER*>(m_data + sizeof(UINT));
	M_ASSERT(m_header->size == sizeof(DDS_HEADER) && m_header->ddsPixelFormat.size == sizeof(DDS_PIXELFORMAT),
		"DDS header information corrupt.");

	const ptrdiff_t ddsDataOffset = sizeof(UINT) + sizeof(DDS_HEADER);
	m_offset = ddsDataOffset;
	m_size = m_size - ddsDataOffset;
	m_format = ConvertFourCCToDXGI(m_header->ddsPixelFormat.fourCC);
}

DXGI_FORMAT DDSLoad::ConvertFourCCToDXGI(uint32_t _fourcc)
{
	const char* fourcc = reinterpret_cast<const char*>(&_fourcc);

	if (strcmp(fourcc, "DXT1")) return DXGI_FORMAT_BC1_UNORM;
	if (strcmp(fourcc, "DXT3")) return DXGI_FORMAT_BC2_UNORM;
	if (strcmp(fourcc, "DXT5")) return DXGI_FORMAT_BC3_UNORM;
	if ((strcmp(fourcc, "ATI1")) ||
		(strcmp(fourcc, "BC4U")))
		return DXGI_FORMAT_BC4_UNORM;
	if ((strcmp(fourcc, "ATI2")) ||
		(strcmp(fourcc, "BC5U")))
		return DXGI_FORMAT_BC5_UNORM;

	if (strcmp(fourcc, "RGBB")) return DXGI_FORMAT_R8G8_B8G8_UNORM;
	if (strcmp(fourcc, "GRGB")) return DXGI_FORMAT_G8R8_G8B8_UNORM;

	return DXGI_FORMAT_UNKNOWN;
}