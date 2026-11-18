#include "ShaderSkybox.h"
#include "Renderer.h"

ShaderSkybox::ShaderSkybox()
{
    SetShaderFile(L"Skybox.hlsl");
}

void ShaderSkybox::CreatePipelineState()
{
    VTX_Pos* vtx = new VTX_Pos();
    SetVertexType(vtx);
    SetInputElementDescs(vtx->InputElementDescs);
    SetInputElemDescSize(_countof(vtx->InputElementDescs));
    SetConstantBufferSize(sizeof(CB_Skybox));
    Shader::CreatePipelineState();
}

void ShaderSkybox::CreateConstantBuffer(UINT64 _bufferSize)
{
    Shader::CreateConstantBuffer(sizeof(CB_Skybox));
}

void ShaderSkybox::SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def)
{
    Shader::SetPipelineState(_renderable, EN_PSD_CULL_FRONT);

    auto r = &Renderer::GetInstance();
    auto c = r->GetCommands()->GetCommandList().Get();
    auto cbData = reinterpret_cast<CB_Skybox*>(ConstantBufferAddress());
    XMStoreFloat4x4(&cbData->View, XMMatrixTranspose(Camera::Current()->GetView()));
    XMStoreFloat4x4(&cbData->Projection, XMMatrixTranspose(Camera::Current()->GetProjection()));
}
