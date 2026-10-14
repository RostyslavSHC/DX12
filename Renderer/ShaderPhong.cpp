
#include "ShaderPhong.h"

ShaderPhong::ShaderPhong()
{
    SetShaderFile(L"phong.hlsl");
}

void ShaderPhong::CreatePipelineState()
{
    VTX_PosNormTex vtx = VTX_PosNormTex();
    SetInputElementDescs(vtx.InputElementDescs);
    SetInputElemDescSize(_countof(vtx.InputElementDescs));
    Shader::CreatePipelineState();
}
