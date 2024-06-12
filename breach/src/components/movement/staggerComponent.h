#pragma once

#include "shade/game/entity/component/component.h"

class StaggerComponent : public Shade::Component
{
public:
    StaggerComponent();

    void Update(float deltaSeconds) override;

    bool IsStaggering() const;
    void TryStagger(float time);
    
    void DisableStagger();
    void EnableStagger();

private:
    float mStaggerTimer = 0.f;

    bool mCanStagger = true;
};