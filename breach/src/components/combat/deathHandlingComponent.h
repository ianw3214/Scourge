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
public:
    static const std::string ComponentID;
#ifdef BUILD_BREACH_EDITOR
public:
    virtual const char* GetComponentID() const { return ComponentID.c_str(); }
    virtual const char* GetDisplayName() const override { return "Death Handling Component"; }
    virtual void ShowImguiDetails() override;
    virtual void SaveToKeyValueFile(Shade::KeyValueFile& file) const override;
#endif
public:
    static DeathHandlingComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
public:
    DeathHandlingComponent();
    DeathHandlingComponent(const std::string& mDieLeftAnim, const std::string& mDieRightAnim);

    virtual void Update(float deltaSeconds) override;

    void HandleDeath(float deathTime);

private:
    float mDeleteTimer = 0.f;

    std::string mDieLeftAnim = DeathHandlingComponentDefaults::dieLeftAnim;
    std::string mDieRightAnim = DeathHandlingComponentDefaults::dieRightAnim;
};