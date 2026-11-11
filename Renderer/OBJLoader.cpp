
#include "OBJLoader.h"

void OBJLoader::ReadFile(const string& _filename)
{
    FILE* file = nullptr;
    M_ASSERT(fopen_s(&file, _filename.c_str(), "rb") == 0, "Failed to open file.");
    M_ASSERT(file != nullptr, "Failed to open file.");
    M_ASSERT(fseek(file, 0, SEEK_END) == 0, "Failed to seek to end of file.");
    size_t size = ftell(file);
    M_ASSERT(size >= 0, "Failed to tell file size.");
    M_ASSERT(fseek(file, 0, SEEK_SET) == 0, "Failed to seek to beginning of file.");

    if (_filename.ends_with(".obj"))
    {
        m_fileBuffer = vector<uint8_t>(size);
        size_t bytesRead = fread(m_fileBuffer.data(), 1, m_fileBuffer.size(), file);
        M_ASSERT(bytesRead == m_fileBuffer.size(), "Failed to read entire obj file.");
    }
    else if (_filename.ends_with(".mtl"))
    {
        m_mtlFileBuffer = vector<uint8_t>(size);
        size_t bytesRead = fread(m_mtlFileBuffer.data(), 1, m_mtlFileBuffer.size(), file);
        M_ASSERT(bytesRead == m_mtlFileBuffer.size(), "Failed to read entire material file.");
    }
    fclose(file);
}

std::vector<std::string> OBJLoader::SplitString(const std::string& _str, char _delimiter)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = 0;

    while ((end = _str.find(_delimiter, start)) != std::string::npos)
    {
        result.push_back(_str.substr(start, end - start));
        start = end + 1;
    }

    // Add the last part
    result.push_back(_str.substr(start));

    return result;
}

void OBJLoader::ProcessOBJ()
{
    size_t start = 0;
    size_t size = m_fileBuffer.size();

    for (size_t i = 0; i < size; ++i)
    {
        // Look for newline characters
        if (m_fileBuffer[i] == '\n' || m_fileBuffer[i] == '\r')
        {
            // Handle Windows CRLF (\r\n)
            size_t lineEnd = i;
            if (m_fileBuffer[i] == '\r' && (i + 1 < size) && m_fileBuffer[i + 1] == '\n')
                ++i;

            string line(m_fileBuffer.begin() + start, m_fileBuffer.begin() + lineEnd);
            if (!line.empty())
            {
                if (line.starts_with("v "))
                {
                    line.erase(0, 1);
                    while (line.front() == ' ') line.erase(0, 1);
                    auto tokens = SplitString(line, ' ');
                    m_positions.push_back({ stof(tokens[0]), stof(tokens[1]), stof(tokens[2]) });
                }
                else if (line.starts_with("vn"))
                {
                    line.erase(0, 2);
                    while (line.front() == ' ') line.erase(0, 1);
                    auto tokens = SplitString(line, ' ');
                    m_normals.push_back({ stof(tokens[0]), stof(tokens[1]), stof(tokens[2]) });
                }
                else if (line.starts_with("vt"))
                {
                    line.erase(0, 2);
                    while (line.front() == ' ') line.erase(0, 1);
                    auto tokens = SplitString(line, ' ');
                    m_texcoords.push_back({ stof(tokens[0]), -1 * stof(tokens[1]), stof(tokens[2]) });
                }
                else if (line[0] == 'f')
                {
                    line.erase(0, 1);
                    while (line.front() == ' ') line.erase(0, 1);
                    auto tokens = SplitString(line, ' ');
                    vector<Vertex> faceVertices;
                    for (auto& token : tokens)
                    {
                        if (token.size() == 0) continue;
                        auto indices = SplitString(token, '/');
                        Vertex v = { m_positions[stoi(indices[0]) - 1], m_texcoords[stoi(indices[1]) - 1],
                            m_normals[stoi(indices[2]) - 1] };
                        faceVertices.push_back(v);
                    }
                    m_vertices.push_back(faceVertices[0]);
                    m_vertices.push_back(faceVertices[1]);
                    m_vertices.push_back(faceVertices[2]);
                }
                else if (line._Starts_with("mtllib"))
                {
                    string materialFileName = line.substr(7, line.size() - 1);
                    materialFileName = "..\\Assets\\Models\\" + materialFileName;
                    ReadFile(materialFileName);
                    ProcessMaterials();
                }
            }

            start = i + 1; // move start to the next character after newline
        }
    }

    // Handle the last line if buffer doesn't end with newline
    if (start < size)
    {
        string line(m_fileBuffer.begin() + start, m_fileBuffer.end());
    }

    m_positions.clear();
    m_normals.clear();
    m_texcoords.clear();
}

void OBJLoader::ProcessMaterials()
{
    size_t start = 0;
    size_t size = m_mtlFileBuffer.size();

    for (size_t i = 0; i < size; ++i)
    {
        // Look for newline characters
        if (m_mtlFileBuffer[i] == '\n' || m_mtlFileBuffer[i] == '\r')
        {
            // Handle Windows CRLF (\r\n)
            size_t lineEnd = i;
            if (m_mtlFileBuffer[i] == '\r' && (i + 1 < size) && m_mtlFileBuffer[i + 1] == '\n')
                ++i;

            string line(m_mtlFileBuffer.begin() + start, m_mtlFileBuffer.begin() + lineEnd);
            while (line[0] == ' ' || line[0] == '\t') line.erase(0, 1); // Remove initial spaces or tabs
            if (!line.empty())
            {
                TryExtractMap(line, "map_Ka ", m_mapAmbient);
                TryExtractMap(line, "map_Kd ", m_mapDiffuse);
                TryExtractMap(line, "map_Ks ", m_mapSpecular);
                TryExtractMap(line, "map_Bump ", m_mapNormal);
            }

            start = i + 1; // move start to the next character after newline
        }
    }
}

void OBJLoader::TryExtractMap(string _line, string _signature, string& _map)
{
    if (_line.starts_with(_signature))
    {
        _line.erase(0, _signature.size());
        std::size_t found_pos = _line.find_last_of('\\');
        if (found_pos != std::string::npos)
        {
            _line = _line.substr(found_pos + 1);
        }
        _map = _line;
    }
}
