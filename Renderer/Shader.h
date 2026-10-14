#pragma once

#include "StandardIncludes.h"
#include "VertexStructs.h"

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 texcoords;
};

struct alignas(256) ConstantBuffer
{
	XMFLOAT4X4 WVP;
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
	void SetInputElementDescs(D3D12_INPUT_ELEMENT_DESC* _inputElementDescs) { m_inputElementDescs = _inputElementDescs; }
	void SetInputElemDescSize(unsigned int _size) { m_inputElemDescSize = _size; }
	void SetShaderFile(LPCWSTR _shaderFile) { m_shaderFile = _shaderFile; }

	// Methods
	void Initialize();
	void SetPipelineState();
	unsigned int NextSRVIndex();

protected:
	virtual void CreatePipelineState();

private:
	// Methods
	void CompileShaders();
	void CreateRootSignature();
	//void CreatePipelineState();
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
	D3D12_INPUT_ELEMENT_DESC* m_inputElementDescs;
	unsigned int m_inputElemDescSize;
	LPCWSTR m_shaderFile;
};