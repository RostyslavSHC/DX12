#pragma once

#include "Unit.h"

class NPC : public Unit
{
public:
    // Constructors/Destructors
    NPC();
    virtual ~NPC();

    // Accessors
    bool IsTagged() const;
    void SetTagged(bool _tagged);

    // Methods
    void Update(XMFLOAT3 _playerPosition, float _deltaTime);

private:
    // Members
    bool m_tagged;
    float m_speed;
    float m_preferredDistance;
};