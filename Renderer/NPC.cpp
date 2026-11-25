#include "NPC.h"

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

    // Update the NPC position
    position.x += direction.x * movement;
    position.y += direction.y * movement;

    SetPosition(position);
}