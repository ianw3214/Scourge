#pragma once
#ifdef DEBUG_BREACH

#include "debug/util.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/entity.h"

class BasicDebugComponent : public Shade::Component
{
public:
    // ======================================
    void Update(float deltaSeconds) override {
        // Draw the position of the entity
        DebugUtils::DrawDebugLine(Shade::Vec2{ mEntityRef->GetPositionX(), mEntityRef->GetPositionY()}, 10.f);
    }
};

#endif