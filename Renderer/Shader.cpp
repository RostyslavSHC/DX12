#include "Shader.h"

Shader::Shader()
{
	m_rootSignature = nullptr;
	m_pipelineState = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_constantBuffer = nullptr;
	m_constantData = nullptr;
}

void Shader::Initialize(LPCWSTR _shaderFile, ID3D12Device* _device)
{
	CompileShaders(_shaderFile);
	CreateRootSignature(_device);
	CreatePipelineState(_device);
	CreateConstantBuffer(_device);
}

void Shader::CompileShaders(LPCWSTR _shaderFile)
{
#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif
	ID3DBlob* errorBlob = nullptr;
	auto res = D3DCompileFromFile(_shaderFile, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &m_vertexShader, &errorBlob);
	M_ASSERT(SUCCEEDED(res), (const char*)errorBlob->GetBufferPointer());
	res = D3DCompileFromFile(_shaderFile, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &m_pixelShader, &errorBlob);
	M_ASSERT(SUCCEEDED(res), (const char*)errorBlob->GetBufferPointer());
}


void Shader::CreateRootSignature(ID3D12Device* _device)
{
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	CD3DX12_ROOT_PARAMETER rootParameters[1] = { { } };
	rootParameters[0].InitAsConstantBufferView(0);	// b0

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	auto res = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signature,
		&error);

	M_ASSERT(SUCCEEDED(res), "Failed to serialize root signature");

	res = _device->CreateRootSignature(
		0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature));

	M_ASSERT(SUCCEEDED(res), "Failed to create root signature");
}






void Shader::CreatePipelineState(ID3D12Device* _device)
{
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
	//psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	auto res = _device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	M_ASSERT(SUCCEEDED(res), "Failed to create graphics pipeline state");
}








void Shader::CreateConstantBuffer(ID3D12Device* _device)
{
	// Create the constant buffer.
	const UINT64 constantBufferSize = ((sizeof(ConstantBuffer) * Renderer::GetFrameCount()) + 255) & ~255;
	const CD3DX12_HEAP_PROPERTIES constantBufferHeapProps(D3D12_HEAP_TYPE_UPLOAD);
	const CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);
	M_ASSERT(SUCCEEDED(_device->CreateCommittedResource(&constantBufferHeapProps, D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&m_constantBuffer))),
		"Failed to commit constant buffer resource.");

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;	// one buffer
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	auto res = _device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_cbvHeap));

	// Describe and create a constant buffer view.
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constantBufferSize;
	_device->CreateConstantBufferView(&cbvDesc, m_cbvHeap->GetCPUDescriptorHandleForHeapStart());

	CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
	M_ASSERT(SUCCEEDED(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_constantData))), "Failed to map constant buffer.");
}

void Shader::SetPipelineState(ID3D12GraphicsCommandList* _commandList, UINT _frameIndex)
{
	_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	ID3D12DescriptorHeap* heaps[] = { m_cbvHeap.Get() };
	_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	_commandList->SetGraphicsRootConstantBufferView(0, m_constantBuffer->GetGPUVirtualAddress() + _frameIndex * sizeof(ConstantBuffer));
}



/*
void Shader::SetPipelineState(ID3D12GraphicsCommandList* _commandList)
{
	_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
}
*/