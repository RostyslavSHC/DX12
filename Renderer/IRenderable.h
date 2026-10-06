#pragma once

class CommandController;
class Shader;
class Texture;
using namespace DirectX;

class IRenderable
{
public:
	// Constructors/Destructors
	IRenderable() { m_shader = nullptr; m_texture = nullptr; }
	virtual ~IRenderable() {}

	// Accessors
	Shader* GetShader() { return m_shader; }
	void SetShader(Shader* _shader) { m_shader = _shader; }
	Texture* GetTexture() { return m_texture; }
	void SetTexture(Texture* _texture) { m_texture = _texture; }

	// Methods
	virtual void Render(XMMATRIX _viewProj) = 0;

private:
	// Members
	Shader* m_shader;
	Texture* m_texture;
};