#include "ShaderPositionColor.h"
#include "Renderer.h"

ShaderPositionColor::ShaderPositionColor()
{
    SetShaderFile(L"positionColor.hlsl"); // Use the position-based color shader
}

void ShaderPositionColor::CreatePipelineState()
{
    VTX_PosNormTex* vtx = new VTX_PosNormTex();
    SetVertexType(vtx);
    SetInputElementDescs(vtx->InputElementDescs);
    SetInputElemDescSize(_countof(vtx->InputElementDescs));
    SetConstantBufferSize(sizeof(ConstantBuffer));
    Shader::CreatePipelineState();
}

void ShaderPositionColor::CreateConstantBuffer(UINT64 _bufferSize)
{
    Shader::CreateConstantBuffer(sizeof(ConstantBuffer));
}

void ShaderPositionColor::SetPipelineState()
{
    Shader::SetPipelineState();
}
