#pragma once

class Shader;

class IRenderable
{
public:
	// Constructors/Destructors
	IRenderable() { m_shader = nullptr; }

	// Accessors
	Shader* GetShader() { return m_shader; }
	void SetShader(Shader* _shader) { m_shader = _shader; }

	// Methods
	virtual void Render(ID3D12GraphicsCommandList* _commandList) = 0;

private:
	// Members
	Shader* m_shader;
};