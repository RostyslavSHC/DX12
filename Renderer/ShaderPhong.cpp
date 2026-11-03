
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
    auto cbData = reinterpret_cast<CB_Phong*>(ConstantBufferAddress());
    FXMVECTOR lightPos = FXMVECTOR{ 0.0f, 0.5f, -1.0f, 0.0f };
    XMStoreFloat4(&cbData->material.DiffuseColor, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat(&cbData->material.SpecularStrength, FXMVECTOR{ 4.0f });
    XMStoreFloat4(&cbData->material.SpecularColor, FXMVECTOR{ 2.0f, 0.0f, 0.0f, 1.0f });
    for (UINT count = 0; count < 4; count++)
    {
        FXMVECTOR origin = { -0.8f + (0.5f * count), 0, 0 };
        XMStoreFloat4(&cbData->lights[count].AmbientColor, FXMVECTOR{ 0.05f, 0.05f, 0.05f, 1.0f });
        XMStoreFloat3(&cbData->lights[count].Position, lightPos);
        XMStoreFloat4(&cbData->lights[count].Color, FXMVECTOR{ 0.0f + (count * 0.25f), 1.0f, 0.0f, 1.0f });
        XMStoreFloat3(&cbData->lights[count].ConstLinQuad, FXMVECTOR{ 1.0f, 0.09f, 0.32f });
        XMStoreFloat3(&cbData->lights[count].SpotDirection, origin - lightPos);
        XMStoreFloat2(&cbData->lights[count].SpotConeFalloff, FXMVECTOR{ XMConvertToRadians(10), 100.0f });
    }
    cbData->LightCount = 4;

    XMStoreFloat4(&cbData->CameraPosition, FXMVECTOR{ 0.0f, 1.0f, -3.0f });
}

