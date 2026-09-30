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
	XMMATRIX GetViewProjection() const
	{
		return XMLoadFloat4x4(&m_view) * XMLoadFloat4x4(&m_projection); // Combine the view and projection matrices
	}

private:
	XMFLOAT4X4 m_projection;// Projection matrix
	XMFLOAT4X4 m_view; // View matrix
};