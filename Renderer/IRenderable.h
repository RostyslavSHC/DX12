
#pragma once

class CommandController;
class Shader;
class Texture;
using namespace DirectX;

class IRenderable
{
public:
    // Constructors/Destructors
    IRenderable() { m_shader = nullptr; m_texDiffuse = nullptr; m_texSpecular = nullptr; m_texNormal = nullptr; m_isInstanced = false;  }
    virtual ~IRenderable() {}

    // Accessors
    Shader* GetShader() { return m_shader; }
    void SetShader(Shader* _shader) { m_shader = _shader; }
    Texture* GetTexDiffuse() { return m_texDiffuse; }
    void SetTexDiffuse(Texture* _texture) { m_texDiffuse = _texture; }
    Texture* GetTexSpecular() { return m_texSpecular; }
    void SetTexSpecular(Texture* _texture) { m_texSpecular = _texture; }
    Texture* GetTexNormal() { return m_texNormal; }
    void SetTexNormal(Texture* _texture) { m_texNormal = _texture; }
    bool IsInstanced() const { return m_isInstanced; }
    void SetIsInstanced(bool _isInstanced) { m_isInstanced = _isInstanced; }



    // Methods
    virtual void Render(XMMATRIX _viewProj) = 0;

private:
    // Members
    Shader* m_shader;
    Texture* m_texDiffuse;
    Texture* m_texSpecular;
    Texture* m_texNormal;
    bool m_isInstanced;
};
