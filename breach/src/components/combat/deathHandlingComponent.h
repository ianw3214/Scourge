#pragma once

#include "shade/game/entity/component/component.h"

#include "shade/file/keyValueFile.h"

#include <string>

// ======================================
namespace DeathHandlingComponentDefaults {
    const std::string dieLeftAnim = "die_left";
    const std::string dieRightAnim = "die_right";
}

// ======================================
// Handles the death of an entity
//  - By default, play a specified animation and mark the entity for remove after a timer
class DeathHandlingComponent : public Shade::Component
{
#ifdef BUILD_BREACH_EDITOR
public:
    const char* GetDisplayName() override { return "Death Handling Component"; }
    void ShowImguiDetails() override;
#endif
public:
    static DeathHandlingComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
public:
    DeathHandlingComponent();
    DeathHandlingComponent(const std::string& mDieLeftAnim, const std::string& mDieRightAnim);

    void Update(float deltaSeconds) override;

    void HandleDeath(float deathTime);

private:
    float mDeleteTimer = 0.f;

    std::string mDieLeftAnim = DeathHandlingComponentDefaults::dieLeftAnim;
    std::string mDieRightAnim = DeathHandlingComponentDefaults::dieRightAnim;
};