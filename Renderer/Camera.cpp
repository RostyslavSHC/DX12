#include "Camera.h"

Camera* Camera::s_current;

Camera::Camera()
{
    m_projection = {};
    m_view = {};
    m_position = {};
    m_lookAt = {};
    m_rotation = {};
    m_angle = 0;
}

Camera::Camera(int _width, int _height)
{
    m_position = {};
    m_lookAt = {};
    m_rotation = {};
    m_angle = 0;

    XMMATRIX projection = XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f),
        (float)_width / (float)_height,
        0.1f,
        100.0f);
    XMStoreFloat4x4(&m_projection, projection);

    // Or, for an ortho camera
    // m_projection = XMMatrixOrthographicLH(_width, _height, 0.1f, 100.f);

    // Camera matrix
    m_position = { 0.0f, 1.0f, -3.0f }; // Camera is at (0,1,-3), in World Space
    m_lookAt = { 0.0f, 0.0f, 0.0f }; // and looks at the origin
    XMVECTOR camUp = { 0.0f, 1.0f, 0.0f }; // Head is up (set to 0,-1,0 to look upside-down)
    XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_lookAt), camUp);
    XMStoreFloat4x4(&m_view, view);
}

Camera::~Camera()
{
}

void Camera::Rotate()
{
    // https://gamedev.stackexchange.com/questions/9607/moving-an-object-in-a-circular-path
    m_angle += 0.1f;
    m_lookAt.x = cos(DirectX::XMConvertToRadians(m_angle)) * 100;
    m_lookAt.z = sin(DirectX::XMConvertToRadians(m_angle)) * 100;

    // Camera matrix
    XMVECTOR camUp = { 0.0f, 1.0f, 0.0f }; // Head is up (set to 0,-1,0 to look upside-down)
    XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_lookAt), camUp);
    XMStoreFloat4x4(&m_view, view);
}





void Camera::SetView(XMFLOAT3 _position, XMFLOAT3 _lookAt, XMFLOAT3 _up)
{
    m_position = _position;
    m_lookAt = _lookAt;
    XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_lookAt), XMLoadFloat3(&_up));
    XMStoreFloat4x4(&m_view, view);
}
