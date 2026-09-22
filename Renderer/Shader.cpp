#include "Shader.h"

Shader::Shader()
{
	m_rootSignature = nullptr;
	m_pipelineState = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
}

void Shader::Initialize(LPCWSTR _shaderFile, ID3D12Device* _device)
{
	CompileShaders(_shaderFile);
	CreatePipelineState(_device);
}

void Shader::CompileShaders(LPCWSTR _shaderFile)
{
#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	auto res = D3DCompileFromFile(_shaderFile, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &m_vertexShader, nullptr);
	M_ASSERT(SUCCEEDED(res), "Failed to compile vertex shader.");
	res = D3DCompileFromFile(_shaderFile, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &m_pixelShader, nullptr);
	M_ASSERT(SUCCEEDED(res), "Failed to compile pixel shader.");
}







void Shader::CreatePipelineState(ID3D12Device* _device)
{
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	auto res = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	M_ASSERT(SUCCEEDED(res), "Failed to serialize root signature");
	res = _device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	M_ASSERT(SUCCEEDED(res), "Failed to create root signature");

	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vertexShader.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_pixelShader.Get());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	res = _device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	M_ASSERT(SUCCEEDED(res), "Failed to create graphics pipeline state");
}


void Shader::SetPipelineState(ID3D12GraphicsCommandList* _commandList)
{
	_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
}