#include "deathHandlingComponent.h"

#include "shade/file/keyValueFile.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"

#include "components/facingComponent.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#ifdef BUILD_BREACH_EDITOR
// ======================================
void DeathHandlingComponent::ShowImguiDetails()
{
    ImGui::InputText("die left anim", &mDieLeftAnim);
    ImGui::InputText("die right anim", &mDieRightAnim);
}

// ======================================
void DeathHandlingComponent::SaveToKeyValueFile(Shade::KeyValueFile& file) const
{
    file.AddStringEntry("die_left", mDieLeftAnim);
    file.AddStringEntry("die_right", mDieRightAnim);
}
#endif

// ======================================
DeathHandlingComponent* DeathHandlingComponent::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    if (handle.IsValid())
    {
        std::string dieLeftAnim = DeathHandlingComponentDefaults::dieLeftAnim;
        std::string dieRightAnim = DeathHandlingComponentDefaults::dieRightAnim;
        while (handle.IsValid())
        {
            if (handle.GetKey() == "die_left")
            {
                dieLeftAnim = handle.TryGetString(dieLeftAnim);
            }
            if (handle.GetKey() == "die_right")
            {
                dieRightAnim = handle.TryGetString(dieRightAnim);
            }
            handle.ToNext();
        }

        return new DeathHandlingComponent(dieLeftAnim, dieRightAnim);
    }   

    return new DeathHandlingComponent(); 
}

// ======================================
DeathHandlingComponent::DeathHandlingComponent()
{

}

// ======================================
DeathHandlingComponent::DeathHandlingComponent(const std::string& dieLeftAnim, const std::string& dieRightAnim)
    : mDieLeftAnim(dieLeftAnim), mDieRightAnim(dieRightAnim)
{

}

// ======================================
void DeathHandlingComponent::Update(float deltaSeconds) 
{
    if (mDeleteTimer > 0.f)
    {
        mDeleteTimer -= deltaSeconds;
        if (mDeleteTimer <= 0.f)
        {
            mEntityRef->MarkForDelete();
        }
    }
}

// ======================================
void DeathHandlingComponent::HandleDeath(float deathTime)
{
    mDeleteTimer = deathTime;

    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? mDieRightAnim : mDieLeftAnim);
}