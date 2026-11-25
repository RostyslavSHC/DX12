#pragma once

#include "Unit.h"
#include "ITaggable.h"


class NPC : public Unit, public ITaggable
{
public:
    // Constructors/Destructors
    NPC();
    virtual ~NPC();

    // Accessors
    virtual bool IsTagged() const override;
    void SetTagged(bool _tagged);

    // Methods
    void Update(XMFLOAT3 _playerPosition, float _deltaTime);
    void CheckTag(XMFLOAT3 _playerPosition);
    virtual void Render(XMMATRIX _viewProj) override;

private:
    // Members
    bool m_tagged;
    float m_speed;
    float m_preferredDistance;
};