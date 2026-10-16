#pragma once

#include "StandardIncludes.h"

class Unit : public Mesh
{
public:
	// Constructors/Destructors
	Unit();
	virtual ~Unit();

	// Accessors
	XMFLOAT3 GetPosition() const;
	void SetPosition(XMFLOAT3 _position);

	XMFLOAT3 GetRotation() const;
	void SetRotation(XMFLOAT3 _rotation);

	XMFLOAT3 GetScale() const;
	void SetScale(XMFLOAT3 _scale);

	// Methods
	virtual void Render(XMMATRIX _viewProj) override;

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;
};