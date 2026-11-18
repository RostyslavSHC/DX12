#include "Shader.h"
#include "Renderer.h"

Shader::Shader()
{
	m_rootSignature = nullptr;
	m_pipelineState = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_constantBuffer = nullptr;
	m_constantData = nullptr;
	m_constantBufferSize = 0;
	m_srvIndex = 0;
	m_shaderFile = nullptr;
	m_inputElemDescSize = 0;
	m_inputElementDescs = nullptr;
	m_cbSizeAligned = 0;
	m_vertexType = nullptr;
	m_currentObjectIndex = 0;

}

unsigned int Shader::NextSRVIndex()
{
	m_srvIndex++;
	return m_srvIndex;
}

void Shader::Initialize()
{
	CompileShaders();
	CreateRootSignature();
	CreatePipelineState();
	CreateConstantBuffer(sizeof(ConstantBuffer));
}

void Shader::CompileShaders()
{
#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif
	ID3DBlob* errorBlob = nullptr;
	auto res = D3DCompileFromFile(m_shaderFile, nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &m_vertexShader, &errorBlob);
	M_ASSERT(SUCCEEDED(res), (const char*)errorBlob->GetBufferPointer());
	res = D3DCompileFromFile(m_shaderFile, nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &m_pixelShader, &errorBlob);
	M_ASSERT(SUCCEEDED(res), (const char*)errorBlob->GetBufferPointer());
}

void Shader::CreateRootSignature()
{
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	CD3DX12_ROOT_PARAMETER rootParameters[2] = { {},{} };
	CD3DX12_DESCRIPTOR_RANGE srvRange = {};
	srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_TEXTURES, 0); // t0..MAX_TEXTURES-1
	rootParameters[0].InitAsConstantBufferView(0); // b0
	rootParameters[1].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL); // t0..MAX_TEXTURES-1

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = sampler.AddressV = sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.ShaderRegister = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 1, &sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	auto res = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	M_ASSERT(SUCCEEDED(res), (const char*)error->GetBufferPointer());
	auto device = Renderer::GetInstance().GetDevice().Get();
	res = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	M_ASSERT(SUCCEEDED(res), (const char*)error->GetBufferPointer());
}

void Shader::CreatePipelineState()
{
	/*
	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	*/

	// Create depth stencil state
	D3D12_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	dsDesc.StencilEnable = FALSE;

	// Create blending state
	CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	// Color blending
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	// Alpha blending
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;

	D3D12_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	rasterDesc.CullMode = D3D12_CULL_MODE_NONE; // Disable culling
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthClipEnable = TRUE;


	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { m_inputElementDescs, m_inputElemDescSize };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vertexShader.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_pixelShader.Get());
	psoDesc.RasterizerState = rasterDesc;
	// psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = dsDesc;
	psoDesc.BlendState = blendDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	auto device = Renderer::GetInstance().GetDevice().Get();
	auto res = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	M_ASSERT(SUCCEEDED(res), "Failed to create graphics pipeline state");
}

void Shader::CreateConstantBuffer(UINT64 _bufferSize)
{
	auto r = &Renderer::GetInstance();
	auto device = r->GetDevice().Get();

	// Create the constant buffer.
	const UINT64 cbSizeAligned = Align256((UINT)_bufferSize);
	m_cbSizeAligned = cbSizeAligned;
	const UINT64 totalSize = cbSizeAligned * r->GetFrameCount() * MAX_OBJECTS;
	const CD3DX12_HEAP_PROPERTIES constantBufferHeapProps(D3D12_HEAP_TYPE_UPLOAD);
	const CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(totalSize);
	M_ASSERT(SUCCEEDED(device->CreateCommittedResource(&constantBufferHeapProps, D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&m_constantBuffer))),
		"Failed to commit constant buffer resource.");

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = MAX_TEXTURES; // cbv, srv0...MAX_TEXTURES-1
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	auto res = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descrHeap));

	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	M_ASSERT(SUCCEEDED(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_constantData))),
		"Failed to map constant buffer.");
}

void Shader::SetPipelineState(IRenderable* _renderable)
{
	auto r = &Renderer::GetInstance();
	auto c = r->GetCommands()->GetCommandList().Get();
	c->SetPipelineState(m_pipelineState.Get());
	c->SetGraphicsRootSignature(m_rootSignature.Get());
	ID3D12DescriptorHeap* heaps[] = { m_descrHeap.Get() };
	c->SetDescriptorHeaps(_countof(heaps), heaps);

	// Set SRV table
	UINT descriptorSize = r->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_descrHeap->GetGPUDescriptorHandleForHeapStart(), 1, descriptorSize);
	r->GetCommands()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvHandle);
}











UINT64 Shader::ConstantBufferOffset()
{
	auto r = &Renderer::GetInstance();
	return (r->GetFrameIndex() * MAX_OBJECTS * m_cbSizeAligned) + (m_currentObjectIndex * m_cbSizeAligned);
}

BYTE* Shader::ConstantBufferAddress()
{
	return reinterpret_cast<BYTE*>(m_constantData) + ConstantBufferOffset();
}

void Shader::BindObjectCB()
{
	auto r = &Renderer::GetInstance();

	D3D12_GPU_VIRTUAL_ADDRESS address =
		m_constantBuffer->GetGPUVirtualAddress() + ConstantBufferOffset();

	r->GetCommands()->GetCommandList()->SetGraphicsRootConstantBufferView(0, address);
}
