#pragma once

#include "shade/game/entity/component/component.h"

#include "components/facingComponent.h"
#include "shade/file/keyValueFile.h"

#include <string>

// ======================================
namespace StaggerComponentDefaults {
    const std::string staggerLeftAnim = "stagger_left";
    const std::string staggerRightAnim = "stagger_right";
}

// ======================================
class StaggerComponent : public Shade::Component
{
#ifdef BUILD_BREACH_EDITOR
public:
    const char* GetDisplayName() override { return "Stagger Component"; }
    void ShowImguiDetails() override;
#endif
public:
    static StaggerComponent* LoadFromFileHandle(Shade::KeyValueHandle handle);
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

    std::string mStaggerLeftAnim = StaggerComponentDefaults::staggerLeftAnim;
    std::string mStaggerRightAnim = StaggerComponentDefaults::staggerRightAnim;
};