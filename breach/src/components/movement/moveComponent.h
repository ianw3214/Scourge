#pragma once

#include "shade/game/entity/component/component.h"

// The base movement component is more or less just a utility to handle fundemental movement logic
//  - Provides the utility of checking if movement is blocked
//    - If the current entity is already in a blocked position, then all movement can be freely done
class BaseMovementComponent : public Shade::Component
{
public:
    // ======================================
    BaseMovementComponent() = default;
    
    void MoveLeft(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    void MoveDown(float distance);
};
