
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
    for (UINT count = 0; count < 5; count++)
    // for (UINT count = 0; count < 1; count++)
    {
        // FXMVECTOR origin = { -0.8f + (0.5f * count), 0, 0 };
        FXMVECTOR origin = { -0.8f + (0.4f * count), 0, 0 };
        XMStoreFloat4(&cbData->lights[count].AmbientColor, FXMVECTOR{ 0.05f, 0.05f, 0.05f, 1.0f });
        XMStoreFloat3(&cbData->lights[count].Position, lightPos);
        XMStoreFloat4(&cbData->lights[count].Color, FXMVECTOR{ 0.0f + (count * 0.25f), 1.0f, 0.0f, 1.0f });
        XMStoreFloat3(&cbData->lights[count].ConstLinQuad, FXMVECTOR{ 1.0f, 0.09f, 0.32f });
        // Tried
        // 1. {1.0f, 0.09f, 0.32f} - Current lighting
        // 2. {1.0f, 0.09f, 0.032f} - Brighter cubes farther from the light
        // 3. {1.0f, 0.7f, 1.8f} - Light fades more quickly
        XMStoreFloat3(&cbData->lights[count].SpotDirection, origin - lightPos);
        XMStoreFloat2(&cbData->lights[count].SpotConeFalloff, FXMVECTOR{ XMConvertToRadians(10), 100.0f });
        // Tried:
		// 1. 10 Degrees, 100.0f - Narrow spotlight, teacher's multiple-light setting 
		// 2. 20 Degrees, 100.0f - Wider illuminated area
		// 3. 30 Degrees, 100.0f - Even wider spotlight
		// 4. 40 Degrees, 10.0f - Softer falloff inside the cone
    }
    cbData->LightCount = 5;

    XMStoreFloat4(&cbData->CameraPosition, FXMVECTOR{ 0.0f, 1.0f, -3.0f });
}

