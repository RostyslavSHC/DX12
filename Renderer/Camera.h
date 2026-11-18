#pragma once

#include "StandardIncludes.h"

class Camera
{
public:
    // Constructors / Destructors
    Camera();
    Camera(int _width, int _height);
    virtual ~Camera();

    // Accessors
    static Camera* Current() { return s_current; }
    static void SetCurrent(Camera* _camera) { s_current = _camera; }

    XMMATRIX GetViewProjection() const
    {
        return XMLoadFloat4x4(&m_view) * XMLoadFloat4x4(&m_projection);
    }

    XMMATRIX GetView() const
    {
        return XMLoadFloat4x4(&m_view);
    }

    XMMATRIX GetProjection() const
    {
        return XMLoadFloat4x4(&m_projection);
    }

    // Methods
    void Rotate();

private:
    // Members
    static Camera* s_current;
    XMFLOAT4X4 m_projection;
    XMFLOAT4X4 m_view;
    XMFLOAT3 m_position;
    XMFLOAT3 m_lookAt;
    XMFLOAT3 m_rotation;
    float m_angle;
};
