#include "NPC.h"
#include "Shader.h"

NPC::NPC()
{
    m_tagged = false;
    m_speed = 5.0f;
    m_preferredDistance = 10.0f;
}

NPC::~NPC()
{
}

bool NPC::IsTagged() const
{
    return m_tagged;
}

void NPC::SetTagged(bool _tagged)
{
    m_tagged = _tagged;
}


void NPC::CheckTag(XMFLOAT3 _playerPosition)
{
    if (m_tagged)
        return;

    // Calculate the distance between the NPC and the player
    XMFLOAT3 position = GetPosition();

    float x = _playerPosition.x - position.x;
    float y = _playerPosition.y - position.y;

    float distance = sqrtf(x * x + y * y);

    // Tag the NPC when the player catches it
    const float tagDistance = 2.5f;

    if (distance <= tagDistance)
    {
        SetTagged(true);
    }
}


void NPC::Update(XMFLOAT3 _playerPosition, float _deltaTime)
{
    // Stop moving when tagged
    if (m_tagged)
        return;

    // Calculate the direction towards the player
    XMFLOAT3 position = GetPosition();

    XMFLOAT3 direction = {
        _playerPosition.x - position.x,
        _playerPosition.y - position.y,
        0.0f
    };

    // Calculate the distance to the player
    float distance = sqrtf(
        direction.x * direction.x +
        direction.y * direction.y);

    // Normalize the direction
    if (distance > 0.0001f)
    {
        direction.x /= distance;
        direction.y /= distance;
    }
    else
    {
        direction = { 1.0f, 0.0f, 0.0f };
    }

    // Calculate the difference from the preferred distance
    float distanceDifference = distance - m_preferredDistance;

    // Stop moving when the preferred distance is reached
    if (fabsf(distanceDifference) < 0.001f)
        return;

    // Calculate movement without overshooting
    float movement = m_speed * _deltaTime;

    if (movement > fabsf(distanceDifference))
        movement = fabsf(distanceDifference);

    // Move away from the player when too close
    if (distanceDifference < 0.0f)
        movement = -movement;


    float angle = atan2f( direction.y * movement, direction.x * movement); // Calculate the rotation based on the movement direction
    SetRotation({ 0.0f, 0.0f, angle - XM_PIDIV2 }); // Rotate around the Z axis


    // Update the NPC position
    position.x += direction.x * movement;
    position.y += direction.y * movement;

    SetPosition(position);
}








void NPC::Render(XMMATRIX _viewProj)
{
    auto cbData = reinterpret_cast<Shader::ConstantBuffer*>(GetShader()->ConstantBufferAddress());

    XMFLOAT3 scale = GetScale();
    XMFLOAT3 rotation = GetRotation();
    XMFLOAT3 position = GetPosition();

    XMMATRIX world = XMMatrixScaling(scale.x, scale.y, scale.z);

    world *= XMMatrixRotationZ(rotation.z);

    world *= XMMatrixTranslation(position.x, position.y, position.z);

    XMStoreFloat4x4(&cbData->World, XMMatrixTranspose(world));

    XMVECTOR determinant; // Required for XMMatrixInverse
    XMMATRIX inverseMatrix = XMMatrixInverse(&determinant, world);
    XMMATRIX inverseTransposeWorld = XMMatrixTranspose(inverseMatrix);
    XMStoreFloat4x4(&cbData->InvWorld, XMMatrixTranspose(inverseTransposeWorld));

    Mesh::Render(world * _viewProj);
}