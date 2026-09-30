#pragma once

class Shader;
using namespace DirectX;

class IRenderable
{
public:
	// Constructors/Destructors
	IRenderable() { m_shader = nullptr; }

	// Accessors
	Shader* GetShader() { return m_shader; }
	void SetShader(Shader* _shader) { m_shader = _shader; }

	// Methods
	virtual void Render(ID3D12GraphicsCommandList* _commandList, XMMATRIX _viewProj) = 0;

private:
	// Members
	Shader* m_shader;
};