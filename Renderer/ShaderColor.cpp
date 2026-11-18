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

void ShaderColor::SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def)
{
    Shader::SetPipelineState(_renderable, EN_PSD_WIREFRAME);

    auto r = &Renderer::GetInstance();
    auto c = r->GetCommands()->GetCommandList().Get();
    auto cbData = reinterpret_cast<CB_Color*>(ConstantBufferAddress());
    XMStoreFloat4(&cbData->Color, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
}
