#include "ShaderDistanceTint.h"

ShaderDistanceTint::ShaderDistanceTint()
{
    SetShaderFile(L"DistanceTint.hlsl");
}

ShaderDistanceTint::~ShaderDistanceTint()
{
}

void ShaderDistanceTint::CreatePipelineState()
{
    SetVertexType(&m_vertexType);
    SetInputElementDescs(m_vertexType.InputElementDescs);
    SetInputElemDescSize(_countof(m_vertexType.InputElementDescs));
    SetConstantBufferSize(sizeof(ConstantBuffer));

    Shader::CreatePipelineState();
}

void ShaderDistanceTint::InitializeTexture(string _filename)
{
    m_texture.Initialize(_filename, this);
}