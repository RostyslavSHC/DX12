
#include "ShaderPhong.h"
#include "Renderer.h"

ShaderPhong::ShaderPhong()
{
    SetShaderFile(L"phong.hlsl");
}

void ShaderPhong::CreatePipelineState()
{
    VTX_PosNormTex vtx = VTX_PosNormTex();
    SetInputElementDescs(vtx.InputElementDescs);
    SetInputElemDescSize(_countof(vtx.InputElementDescs));
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
    uint8_t* base = reinterpret_cast<uint8_t*>(GetConstantData());
    CB_Phong* cbData = reinterpret_cast<CB_Phong*>(
        base + r->GetFrameIndex() * GetConstantBufferSize());
    XMStoreFloat4(&cbData->DiffuseColor, FXMVECTOR{ 1.0f, 0.0f, 0.0f, 1.0f });
}
