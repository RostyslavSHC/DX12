#include "Camera.h"

Camera::Camera()
{
	// Clear matrices
	m_projection = { };
	m_view = { };
}

Camera::Camera(int _width, int _height)
{
	// Create perspective projection matrix
	XMMATRIX projection = XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),
		(float)_width / (float)_height,
		0.1f,
		100.0f);

	XMStoreFloat4x4(&m_projection, projection);

	// Or, for an ortho camera
	// m_projection = XMMatrixOrthographicLH(_width, _height, 0.1f, 100f);

	// Camera matrix
	XMVECTOR camPos = { 0.0f, 0.0f, -6.0f };	// Camera position
	XMVECTOR lookAt = { 0.0f, 0.0f, 0.0f };		// Point camera looks at
	XMVECTOR camUp = { 0.0f, 1.0f, 0.0f };		// Up direction

	XMMATRIX view = XMMatrixLookAtLH(camPos, lookAt, camUp);

	// Store view matrix
	XMStoreFloat4x4(&m_view, view);
}

Camera::~Camera()
{
}