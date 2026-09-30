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






Camera::Camera(int _width, int _height, int _setup)
{
	// Two projection matrices
	XMMATRIX perspective = XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45.0f),
		(float)_width / (float)_height,
		0.1f,
		100.0f);

	XMMATRIX orthographic = XMMatrixOrthographicLH(
		4.0f,
		3.0f,
		0.1f,
		100.0f);

	// View 1 - camera directly in front
	XMVECTOR camPos1 = { 0.0f, 0.0f, -6.0f };
	XMVECTOR lookAt1 = { 0.0f, 0.0f, 0.0f };
	XMVECTOR camUp1 = { 0.0f, 1.0f, 0.0f };

	XMMATRIX view1 = XMMatrixLookAtLH(camPos1, lookAt1, camUp1);

	// View 2 - camera moved to the side
	XMVECTOR camPos2 = { 3.0f, 2.0f, -6.0f };
	XMVECTOR lookAt2 = { 0.0f, 0.0f, 0.0f };
	XMVECTOR camUp2 = { 0.0f, 1.0f, 0.0f };

	XMMATRIX view2 = XMMatrixLookAtLH(camPos2, lookAt2, camUp2);

	// Setup 1: View 1 + Perspective
	if (_setup == 1)
	{
		XMStoreFloat4x4(&m_view, view1);
		XMStoreFloat4x4(&m_projection, perspective);
	}

	// Setup 2: View 1 + Orthographic
	else if (_setup == 2)
	{
		XMStoreFloat4x4(&m_view, view1);
		XMStoreFloat4x4(&m_projection, orthographic);
	}

	// Setup 3: View 2 + Perspective
	else if (_setup == 3)
	{
		XMStoreFloat4x4(&m_view, view2);
		XMStoreFloat4x4(&m_projection, perspective);
	}

	// Setup 4: View 2 + Orthographic
	else
	{
		XMStoreFloat4x4(&m_view, view2);
		XMStoreFloat4x4(&m_projection, orthographic);
	}
}









Camera::~Camera()
{
}