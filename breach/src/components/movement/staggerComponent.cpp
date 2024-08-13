#include "staggerComponent.h"

#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"

#include "components/combat/healthComponent.h"
#include "components/facingComponent.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#ifdef BUILD_BREACH_EDITOR
// ======================================
void StaggerComponent::ShowImguiDetails() 
{
    ImGui::InputText("stagger left anim", &mStaggerLeftAnim);
    ImGui::InputText("stagger right anim", &mStaggerRightAnim);
}
#endif

// ======================================
StaggerComponent* StaggerComponent::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    if (handle.IsValid())
    {
        std::string staggerLeftAnim = StaggerComponentDefaults::staggerLeftAnim;
        std::string mStaggerRightAnim = StaggerComponentDefaults::staggerRightAnim;
        while (handle.IsValid())
        {
            if (handle.GetKey() == "stagger_left")
            {
                staggerLeftAnim = handle.TryGetString();
            }
            if (handle.GetKey() == "stagger_right")
            {
                mStaggerRightAnim = handle.TryGetString();
            }
            handle.ToNext();
        }

        return new StaggerComponent(staggerLeftAnim, mStaggerRightAnim);
    }   

    return new StaggerComponent(); 
}

// ======================================
StaggerComponent::StaggerComponent()
{

}

// ======================================
StaggerComponent::StaggerComponent(const std::string& staggerLeftAnim, const std::string& staggerRightAnim)
    : mStaggerLeftAnim(staggerLeftAnim)
    , mStaggerRightAnim(staggerRightAnim)
{

}

// ======================================
void StaggerComponent::Update(float deltaSeconds) 
{
    if (mStaggerTimer > 0.f)
    {
        HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
        if (health && health->IsDead())
        {
            mStaggerTimer = 0.f;
            return;
        }

        FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
        mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? mStaggerRightAnim : mStaggerLeftAnim, false);

        mStaggerTimer -= deltaSeconds;
    }
}

// ======================================
bool StaggerComponent::IsStaggering() const
{
    return mStaggerTimer > 0.f;
}

// ======================================
void StaggerComponent::TryStagger(float time)
{
    if (!mCanStagger)
    {
        return;
    }

    mStaggerTimer = time;

    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? mStaggerRightAnim : mStaggerLeftAnim, false);

    // TODO: Potentially handle inturrupting actions here as well
    //  - e.g. inturrupted attack
}

// ======================================
// TODO: Potentially refactor to share common code w/ above function
//  - Alternatively, have the attacks handle changing the entities direction and just use the normal TryStagger
void StaggerComponent::TryStaggerInDirection(float time, FacingDirection direction)
{
    if (!mCanStagger)
    {
        return;
    }

    HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
    if (health && health->IsDead())
    {
        return;
    }

    mStaggerTimer = time;

    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(direction == FacingDirection::RIGHT ? mStaggerRightAnim : mStaggerLeftAnim, false);
    // Update the facing to face the direction that the stagger is in
    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    facing->mDirection = direction;

    // TODO: Potentially handle inturrupting actions here as well
    //  - e.g. inturrupted attack
}

// ======================================
void StaggerComponent::DisableStagger()
{
    mCanStagger = false;
}

// ======================================
void StaggerComponent::EnableStagger()
{
    mCanStagger = true;
}