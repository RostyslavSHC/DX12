
#include "Camera.h"

Camera::Camera()
{
    m_projection = {};
    m_view = {};
}

Camera::Camera(int _width, int _height)
{
    XMMATRIX projection = XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f),
        (float)_width / (float)_height,
        0.1f,
        100.0f);
    XMStoreFloat4x4(&m_projection, projection);

    // Or, for an ortho camera
    // m_projection = XMMatrixOrthographicLH(_width, _height, 0.1f, 100.f);

    // Camera matrix
    XMVECTOR camPos = { 0.0f, 1.0f, -3.0f }; // Camera is at (0,0,-1) in World Space
    XMVECTOR lookAt = { 0.0f, 0.0f, 0.0f }; // and looks at the origin
    XMVECTOR camUp = { 0.0f, 1.0f, 0.0f }; // Head is up (set to 0,-1,0 to look upside-down)
    XMMATRIX view = XMMatrixLookAtLH(camPos, lookAt, camUp);
    XMStoreFloat4x4(&m_view, view);
}

Camera::~Camera()
{
}

