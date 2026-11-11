
#pragma once

#include "StandardIncludes.h"

struct VPosition
{
    float x;
    float y;
    float z;
};

struct VNormal
{
    float nx;
    float ny;
    float nz;
};

struct VTexcoord
{
    float u;
    float v;
    float w;
};

struct Vertex
{
    VPosition pos;
    VTexcoord tex;
    VNormal nrm;
};

class OBJLoader
{
public:
    // Accessors
    vector<Vertex>& GetVertices() { return m_vertices; }
    string GetMapAmbient() { return m_mapAmbient; }
    string GetMapDiffuse() { return m_mapDiffuse; }
    string GetMapSpecular() { return m_mapSpecular; }
    string GetMapNormal() { return m_mapNormal; }

    // Methods
    void ReadFile(const string& _filename);
    void ProcessOBJ();
    std::vector<std::string> SplitString(const std::string& _str, char _delimiter);

private:
    // Methods
    void ProcessMaterials();
    void TryExtractMap(string _line, string _signature, string& _map);

    // Members
    vector<uint8_t> m_fileBuffer;
    vector<uint8_t> m_mtlFileBuffer;
    vector<VPosition> m_positions;
    vector<VNormal> m_normals;
    vector<VTexcoord> m_texcoords;
    vector<Vertex> m_vertices;
    string m_mapAmbient;
    string m_mapDiffuse;
    string m_mapSpecular;
    string m_mapNormal;
};
