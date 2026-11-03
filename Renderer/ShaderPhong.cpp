
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

    FXMVECTOR camPos = FXMVECTOR{ 0.5f, 0.5f, -2.0f, 0.0f };
    FXMVECTOR origin = { 0,0,0 };

    // CB_Phong* cbData = reinterpret_cast<CB_Phong*>(&GetConstantData()[r->GetFrameIndex()]);
    XMStoreFloat4(&cbData->material.DiffuseColor, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat(&cbData->material.SpecularStrength, FXMVECTOR{ 4.0f });
    XMStoreFloat4(&cbData->material.SpecularColor, FXMVECTOR{ 2.0f, 0.0f, 0.0f, 1.0f });
    XMStoreFloat4(&cbData->light.AmbientColor, FXMVECTOR{ 0.15f, 0.15f, 0.15f, 1.0f });

    XMStoreFloat3(&cbData->light.Position, camPos);
    XMStoreFloat4(&cbData->light.Color, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat3(&cbData->light.ConstLinQuad, FXMVECTOR{ 1.0f, 0.09f, 0.32f });

	XMStoreFloat3(&cbData->light.SpotDirection, origin - camPos);
	XMStoreFloat2(&cbData->light.SpotConeFalloff, FXMVECTOR{ XMConvertToRadians(15), 100.0f });

    XMStoreFloat4(&cbData->CameraPosition, FXMVECTOR{ 0.0f, 1.0f, -3.0f });
}

