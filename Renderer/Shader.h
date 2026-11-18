#pragma once

#include "StandardIncludes.h"
#include "VertexStructs.h"

using namespace DirectX;

/*
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 texcoords;
};
*/

enum PipelineStateDefs
{
	EN_PSD_DEFAULT = 0,
	EN_PSD_CULL_FRONT,
	EN_PSD_WIREFRAME,
	EN_PSD_COUNT
};

class Shader
{
public:
	// Constructors/Destructors
	Shader();
	virtual ~Shader() {}

	// Constant Buffer
	struct alignas(256) ConstantBuffer
	{
		XMFLOAT4X4 WVP;
		XMFLOAT4X4 World;
		XMFLOAT4X4 InvWorld;
	};

	// Accessors
	ComPtr<ID3D12RootSignature>& GetRootSignature() { return m_rootSignature; }
	ComPtr<ID3D12PipelineState>& GetPipelineState(PipelineStateDefs _def) { return m_pipelineStates[_def]; }
	ComPtr<ID3D12DescriptorHeap>& GetDescrHeap() { return m_descrHeap; }
	ConstantBuffer* GetConstantData() const { return m_constantData; }
	void SetInputElementDescs(D3D12_INPUT_ELEMENT_DESC* _inputElementDescs) { m_inputElementDescs = _inputElementDescs; }
	void SetInputElemDescSize(unsigned int _size) { m_inputElemDescSize = _size; }
	void SetShaderFile(LPCWSTR _shaderFile) { m_shaderFile = _shaderFile; }
	void SetConstantBufferSize(UINT64 _size) { m_constantBufferSize = _size; }
	UINT64 GetConstantBufferSize() const { return m_constantBufferSize; }
	void SetVertexType(VTX_Base* _vertexType) { m_vertexType = _vertexType; }
	VTX_Base* GetVertexType() { return m_vertexType; }
	UINT64 GetCBSizeAligned() const { return m_cbSizeAligned; }
	int GetCurrentObjectIndex() const { return m_currentObjectIndex; }
	void ResetCurrentObjectIndex() { m_currentObjectIndex = 0; }
	void IncrementCurrentObjectIndex() { m_currentObjectIndex++; }


	// Methods
	void Initialize();
	virtual void SetPipelineState(IRenderable* _renderable, PipelineStateDefs _def = EN_PSD_DEFAULT);
	unsigned int NextSRVIndex();
	inline UINT Align256(UINT size)
	{
		return (size + 255) & ~255;
	}
	void BindObjectCB();
	BYTE* ConstantBufferAddress();

	// Members
	static constexpr UINT64 MAX_OBJECTS = 100;
	static constexpr UINT MAX_TEXTURES = 8;

protected:
	virtual void CreatePipelineState();
	virtual void CreateConstantBuffer(UINT64 _bufferSize);

private:
	// Methods
	void CreatePipelineState(PipelineStateDefs _def);
	void CompileShaders();
	void CreateRootSignature();
	UINT64 ConstantBufferOffset();

	// Members
	ComPtr<ID3D12RootSignature> m_rootSignature;
	map<int, ComPtr<ID3D12PipelineState>> m_pipelineStates;
	// ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3DBlob> m_vertexShader;
	ComPtr<ID3DBlob> m_pixelShader;
	ComPtr<ID3D12DescriptorHeap> m_descrHeap;
	ComPtr<ID3D12Resource> m_constantBuffer;
	ConstantBuffer* m_constantData;
	UINT64 m_constantBufferSize;
	unsigned int m_srvIndex;
	D3D12_INPUT_ELEMENT_DESC* m_inputElementDescs;
	unsigned int m_inputElemDescSize;
	LPCWSTR m_shaderFile;
	VTX_Base* m_vertexType;
	UINT64 m_cbSizeAligned;
	int m_currentObjectIndex = 0;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDescBase;
};