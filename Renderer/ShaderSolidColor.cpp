#include "ShaderSolidColor.h"

ShaderSolidColor::ShaderSolidColor()
{
    SetShaderFile(L"ShaderSolidColor.hlsl");
    m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void ShaderSolidColor::SetColor(XMFLOAT4 _color)
{
    m_color = _color;
}

void ShaderSolidColor::SetPipelineState(IRenderable* _renderable,
    PipelineStateDefs _def)
{
    Shader::SetPipelineState(_renderable, _def);

    auto cbData = reinterpret_cast<CB_Color*>(ConstantBufferAddress());
    cbData->Color = m_color;
}