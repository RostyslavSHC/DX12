#include "ShaderColor.h"
#include "Renderer.h"

ShaderColor::ShaderColor()
{
    SetShaderFile(L"color.hlsl");
}

void ShaderColor::CreatePipelineState()
{
    VTX_Pos* vtx = new VTX_Pos();
    SetVertexType(vtx);
    SetInputElementDescs(vtx->InputElementDescs);
    SetInputElemDescSize(_countof(vtx->InputElementDescs));
    SetConstantBufferSize(sizeof(CB_Color));
    Shader::CreatePipelineState();
}

void ShaderColor::CreateConstantBuffer(UINT64 _bufferSize)
{
    Shader::CreateConstantBuffer(sizeof(CB_Color));
}

void ShaderColor::SetPipelineState()
{
    Shader::SetPipelineState();

    auto r = &Renderer::GetInstance();
    auto c = r->GetCommands()->GetCommandList().Get();
    auto cb = GetConstantData()[r->GetFrameIndex()];
    CB_Color* cbData = reinterpret_cast<CB_Color*>(&GetConstantData()[r->GetFrameIndex()]);
    XMStoreFloat4(&cbData->Color, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
}
