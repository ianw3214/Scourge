#include "playerInputComponent.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/game/entity/entity.h"
#include "shade/game/event/event.h"

#include "components/combat/attackComponent.h"
#include "components/facingComponent.h"
#include "components/movement/locomotionComponent.h"

// ======================================
void PlayerInputComponent::Update(float deltaSeconds) {
    LocomotionComponent* locomotion = mEntityRef->GetComponent<LocomotionComponent>();
    AttackComponent* attackComponent = mEntityRef->GetComponent<AttackComponent>();
    if (locomotion == nullptr)
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogWarning("No locomotion component found on entity with PlayerInputComponent");
        return;
    }
    if (attackComponent == nullptr)
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogWarning("No attack component found on entity with PlayerInputComponent");
        return;
    }
    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    if (mEntityRef->GetBooleanEvent("attack").mHeld)
    {
        // TODO: Have these account for input direction before accounting for attack direction
        attackComponent->TryDoAttack(facing->mDirection == FacingDirection::RIGHT ? "attack_right" : "attack_left");
        return;
    }
    if (mEntityRef->GetBooleanEvent("roll").mHeld)
    {
        // TODO: Have these account for input direction before accounting for attack direction
        attackComponent->TryDoAttack(facing->mDirection == FacingDirection::RIGHT ? "dash_right" : "dash_left");
        return;
    }
    // TODO: These should really be int events, since that is how SDL represents the values
    //  - alternatively, noramlize the values to float values
    float controllerMoveHorizontal = mEntityRef->GetIntEvent("move_h").mValue;
    float controllerMoveVertical = mEntityRef->GetIntEvent("move_v").mValue;
    locomotion->mMovingUp = mEntityRef->GetBooleanEvent("move_up").mHeld || controllerMoveVertical < -10000;
    locomotion->mMovingDown = mEntityRef->GetBooleanEvent("move_down").mHeld || controllerMoveVertical > 10000;
    locomotion->mMovingRight = mEntityRef->GetBooleanEvent("move_right").mHeld || controllerMoveHorizontal > 10000;
    locomotion->mMovingLeft = mEntityRef->GetBooleanEvent("move_left").mHeld || controllerMoveHorizontal < -10000;
}