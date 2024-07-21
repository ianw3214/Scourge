#pragma once

#include "shade/game/entity/component/component.h"

#include "components/facingComponent.h"

#include <string>

class StaggerComponent : public Shade::Component
{
public:
    StaggerComponent();
    StaggerComponent(const std::string& staggerLeftAnim, const std::string& staggerRightAnim);

    void Update(float deltaSeconds) override;

    bool IsStaggering() const;
    void TryStagger(float time);
    void TryStaggerInDirection(float time, FacingDirection direction);
    
    void DisableStagger();
    void EnableStagger();

private:
    float mStaggerTimer = 0.f;

    bool mCanStagger = true;

    std::string mStaggerLeftAnim = "stagger_left";
    std::string mStaggerRightAnim = "stagger_right";
};