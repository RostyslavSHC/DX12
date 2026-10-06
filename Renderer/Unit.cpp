#include "Unit.h"

Unit::Unit()
{
	m_position = { 0, 0, 0 };
	m_rotation = { 0, 0, 0 };
	m_scale = { 1, 1, 1 };
}

Unit::~Unit()
{
}

void Unit::Render(XMMATRIX _viewProj)
{
	XMMATRIX world = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	world *= XMMatrixRotationY(m_rotation.y);
	world *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	Mesh::Render(world * _viewProj);
}