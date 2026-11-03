
#include "ShaderPhong.h"
#include "Renderer.h"

ShaderPhong::ShaderPhong()
{
    SetShaderFile(L"phong.hlsl");
}

void ShaderPhong::CreatePipelineState()
{
    VTX_PosNormTex* vtx = new VTX_PosNormTex();
    SetVertexType(vtx);
    SetInputElementDescs(vtx->InputElementDescs);
    SetInputElemDescSize(_countof(vtx->InputElementDescs));
    SetConstantBufferSize(sizeof(CB_Phong));
    Shader::CreatePipelineState();
}

void ShaderPhong::CreateConstantBuffer(UINT64 _bufferSize)
{
    Shader::CreateConstantBuffer(sizeof(CB_Phong));
}


void ShaderPhong::SetPipelineState()
{
    Shader::SetPipelineState();

    auto r = &Renderer::GetInstance();
    auto c = r->GetCommands()->GetCommandList().Get();
    CB_Phong* cbData = reinterpret_cast<CB_Phong*>(&GetConstantData()[r->GetFrameIndex()]);
    XMStoreFloat4(&cbData->material.DiffuseColor, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat(&cbData->material.SpecularStrength, FXMVECTOR{ 16.0f });
    XMStoreFloat4(&cbData->material.SpecularColor, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat4(&cbData->light.AmbientColor, FXMVECTOR{ 0.2f, 0.2f, 0.2f, 1.0f });
    XMStoreFloat3(&cbData->light.Position, FXMVECTOR{ 0.5f, 0.5f, -2.0f, 0.0f });
    XMStoreFloat4(&cbData->light.Color, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat4(&cbData->CameraPosition, FXMVECTOR{ 0.0f, 1.0f, -3.0f });
}

