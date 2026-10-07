#pragma once

#include "StandardIncludes.h"
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 texcoords;
};

// Extended constant buffer so it also contains the y, u, v, invertColors values
struct alignas(256) ConstantBuffer
{
	XMFLOAT4X4 WVP;

	float yValue;
	float uValue;
	float vValue;
	float invertColors; // Bool is 1 byte, but HLSL constant buffer requires 4 bytes. It creates missmatching so easier to use float which is 4 bytes
};

class Shader
{
public:
	// Constructors/Destructors
	Shader();
	virtual ~Shader() {}

	// Accessors
	ComPtr<ID3D12RootSignature>& GetRootSignature() { return m_rootSignature; }
	ComPtr<ID3D12PipelineState>& GetPipelineState() { return m_pipelineState; }
	ComPtr<ID3D12DescriptorHeap>& GetDescrHeap() { return m_descrHeap; }
	ConstantBuffer* GetConstantData() const { return m_constantData; }

	// Methods
	void Initialize(LPCWSTR _shaderFile);
	void SetPipelineState();
	unsigned int NextSRVIndex();

private:
	// Methods
	void CompileShaders(LPCWSTR _shaderFile);
	void CreateRootSignature();
	void CreatePipelineState();
	void CreateConstantBuffer();

	// Members
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3DBlob> m_vertexShader;
	ComPtr<ID3DBlob> m_pixelShader;
	ComPtr<ID3D12DescriptorHeap> m_descrHeap;
	ComPtr<ID3D12Resource> m_constantBuffer;
	ConstantBuffer* m_constantData;
	unsigned int m_srvIndex;
};