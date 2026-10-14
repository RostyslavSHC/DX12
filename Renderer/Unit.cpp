#include "Unit.h"
#include "Renderer.h"
#include "Shader.h"

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
	auto r = &Renderer::GetInstance();
	XMMATRIX world = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	world *= XMMatrixRotationY(m_rotation.y);
	world *= XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	Shader::ConstantBuffer* cbData = &GetShader()->GetConstantData()[r->GetFrameIndex()];
	XMStoreFloat4x4(&cbData->World, XMMatrixTranspose(world));
	XMVECTOR determinant; // Required for XMMatrixInverse
	XMMATRIX inverseMatrix = XMMatrixInverse(&determinant, world);
	XMMATRIX inverseTransposeWorld = XMMatrixTranspose(inverseMatrix);
	XMStoreFloat4x4(&cbData->InvWorld, XMMatrixTranspose(inverseTransposeWorld));

	Mesh::Render(world * _viewProj);
}
