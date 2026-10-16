
#include "ShaderPhong.h"
#include "Renderer.h"

ShaderPhong::ShaderPhong()
{
    SetShaderFile(L"phong.hlsl");
    m_lightPosition = XMFLOAT3(0.5f, 0.5f, -2.0f); // Initial light position
}

void ShaderPhong::SetLightPosition(XMFLOAT3 _position)
{
    m_lightPosition = _position; // Store the updated light position
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
    XMStoreFloat4(&cbData->DiffuseColor, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat4(&cbData->AmbientLight, FXMVECTOR{ 0.2f, 0.2f, 0.2f, 1.0f });
    cbData->LightPosition = m_lightPosition; // Send the current light position to the shader
    XMStoreFloat4(&cbData->LightColor, FXMVECTOR{ 1.0f, 1.0f, 1.0f, 1.0f });
    XMStoreFloat(&cbData->SpecularStrength, FXMVECTOR{ 4.0f });
    XMStoreFloat4(&cbData->SpecularColor, FXMVECTOR{ 1.0f, 0.0f, 0.0f, 0.0f });
    XMStoreFloat4(&cbData->CameraPosition, FXMVECTOR{ 0.0f, 1.0f, -3.0f });
}

