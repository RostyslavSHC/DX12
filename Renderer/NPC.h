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

    // Methods
    void Update(XMFLOAT3 _playerPosition, float _deltaTime, bool _allowTagging);
    virtual void Render(XMMATRIX _viewProj) override;

private:
    // Methods
    void CheckTag(XMFLOAT3 _playerPosition);
    void SetTagged(bool _tagged);

    // Members
    bool m_tagged;
    float m_speed;
    float m_preferredDistance;
};