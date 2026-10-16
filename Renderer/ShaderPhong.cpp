
#include "ShaderPhong.h"
#include "Renderer.h"

ShaderPhong::ShaderPhong()
{
    SetShaderFile(L"phong.hlsl");
    m_lightPosition = XMFLOAT3(0.5f, 0.5f, -2.0f); // Initial light position
    m_specularStrength = 4.0f; // Initial specular strength
    m_specularColor = XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f); // Initial specular color
}

void ShaderPhong::SetLightPosition(XMFLOAT3 _position)
{
    m_lightPosition = _position; // Store the updated light position
}

void ShaderPhong::SetSpecularStrength(float _strength)
{
    m_specularStrength = _strength; // Store the updated specular strength
}

void ShaderPhong::SetSpecularColor(XMFLOAT4 _color)
{
    m_specularColor = _color; // Store the updated specular color
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
    cbData->SpecularStrength = m_specularStrength; // Send the current specular strength to the shader
    cbData->SpecularColor = m_specularColor; // Send the current specular color to the shader
    XMStoreFloat4(&cbData->CameraPosition, FXMVECTOR{ 0.0f, 1.0f, -3.0f });
}

