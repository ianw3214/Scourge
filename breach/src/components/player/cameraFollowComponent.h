#pragma once

#include "shade/game/entity/component/component.h"

class CameraFollowComponent : public Shade::Component
{
public:
    void Update(float deltaSeconds) override;
};