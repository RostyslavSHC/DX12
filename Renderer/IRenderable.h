#pragma once

class CommandController;
class Shader;
class Texture;
using namespace DirectX;

class IRenderable
{
public:
	// Constructors/Destructors
	IRenderable() { m_shader = nullptr; m_texture0 = nullptr; m_texture1 = nullptr; }
	virtual ~IRenderable() {}

	// Accessors
	Shader* GetShader() { return m_shader; }
	void SetShader(Shader* _shader) { m_shader = _shader; }
	Texture* GetTexture0() { return m_texture0; }
	void SetTexture0(Texture* _texture) { m_texture0 = _texture; }
	Texture* GetTexture1() { return m_texture1; }
	void SetTexture1(Texture* _texture) { m_texture1 = _texture; }

	// Methods
	virtual void Render(XMMATRIX _viewProj) = 0;

private:
	// Members
	Shader* m_shader;
	Texture* m_texture0;
	Texture* m_texture1;
};