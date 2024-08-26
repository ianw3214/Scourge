#include "attackComponent.h"

#include "definitions.h"

#include "shade/file/keyValueFile.h"
#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/graphics/flare/flareService.h"
#include "shade/graphics/texture.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

#include "debug/util.h"
#include "components/combat/hitboxComponent.h"
#include "components/combat/healthComponent.h"
#include "components/facingComponent.h"
#include "components/movement/locomotionComponent.h"
#include "components/movement/moveComponent.h"
#include "components/movement/staggerComponent.h"

#include <cassert>

// TODO: Remove - Temporary for determining player or AI
#include "components/ai/stateMachineAIComponent.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

const std::string AttackComponent::ComponentID = "attack";

#ifdef BUILD_BREACH_EDITOR
namespace {
    const char* AttackTargetStrings[] = { "player", "enemy" };
}

// ======================================
void AttackHitInfo::ShowImguiDetails()
{
    ImGui::InputScalar("Trigger anim frame", ImGuiDataType_U32, &mTriggerFrame);
    ImGui::DragFloat("Damage", &mDamage, 1.f, 0.f, 200.f);

    int currIndex = static_cast<int>(mTarget);
    if (ImGui::BeginCombo("target type", AttackTargetStrings[currIndex]))
    {
        for (int n = 0; n < static_cast<int>(AttackTarget::COUNT); n++)
        {
            const bool isSelected = (currIndex == n);
            if (ImGui::Selectable(AttackTargetStrings[n], isSelected))
            {
                mTarget = static_cast<AttackTarget>(n);
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
        }
        ImGui::EndCombo();
    }

    // TODO: Ability to remove boxes
    ImGui::Text("Hit boxes");
    if (ImGui::Button("Add hitbox"))
    {
        mAttackBoxes.emplace_back();
    }
    for (int n = 0; n < mAttackBoxes.size(); n++)
    {
        if (ImGui::TreeNode(std::to_string(n).c_str()))
        {
            AttackHitBox& hitbox = mAttackBoxes[n];
            ImGui::DragFloat("Offset X", &hitbox.mOffsetX);
            ImGui::DragFloat("Offset Y", &hitbox.mOffsetY);
            ImGui::DragFloat("Width", &hitbox.mWidth, 1.f, 0.f, 500.f);
            ImGui::DragFloat("Height", &hitbox.mHeight, 1.f, 0.f, 500.f);
            const bool effectChanged = ImGui::InputText("Effect", &hitbox.mEffectPath);

            if (effectChanged)
            {
                // TODO: Ability to play the attack in editor
            }
            ImGui::TreePop();
        }
    }
    
}

// ======================================
void AttackHitInfo::SaveToKeyValueFile(Shade::KeyValueFile& file) const
{
    file.AddIntEntry("anim_frame", mTriggerFrame);
    file.AddFloatEntry("damage", mDamage);
    file.AddIntEntry("target", static_cast<int>(mTarget));
    if (!mAttackBoxes.empty())
    {
        file.PushList("boxes");
        for (int n = 0; n < mAttackBoxes.size(); ++n)
        {
            const AttackHitBox& hitbox = mAttackBoxes[n];
            file.PushList(std::to_string(n));
            file.AddFloatEntry("width", hitbox.mWidth);
            file.AddFloatEntry("height", hitbox.mHeight);
            file.AddFloatEntry("offset_x", hitbox.mOffsetX);
            file.AddFloatEntry("offset_y", hitbox.mOffsetY);
            if (!hitbox.mEffectPath.empty())
            {
                file.PushList("effect");
                file.AddStringEntry("path", hitbox.mEffectPath);
                file.PopList();
            }
            file.PopList();
        }
        file.PopList();
    }
}

// ======================================
void AttackInfo::ShowImguiDetails()
{
    ImGui::InputText("Animation", &mAnimation);
    ImGui::Checkbox("Disable movement", &mDisableMovement);
    ImGui::Checkbox("Invulnerable", &mInvulnerable);
    ImGui::DragFloat("Duration", &mDuration, 1.f, 0.f, 20.f);
    ImGui::DragFloat("Move speed", &mMoveSpeed, 1.f, 0.f, 1500.f);

    ImGui::Text("Hit definitions");
    if (ImGui::Button("Add hit definition"))
    {
        mHitInfo.emplace_back();
    }
    for (int n = 0; n < mHitInfo.size(); n++)
    {
        if (ImGui::TreeNode(std::to_string(n).c_str()))
        {
            mHitInfo[n].ShowImguiDetails();
            ImGui::TreePop();
        }
    }
}

// ======================================
void AttackInfo::SaveToKeyValueFile(Shade::KeyValueFile& file) const
{
    file.AddStringEntry("anim", mAnimation);
    file.AddIntEntry("disable_movement", static_cast<int>(mDisableMovement));
    file.AddIntEntry("invulnerable", static_cast<int>(mInvulnerable));
    file.AddFloatEntry("duration", mDuration);
    file.AddFloatEntry("speed", mMoveSpeed);
    if (!mHitInfo.empty())
    {
        file.PushList("hits");
        for (int n = 0; n < mHitInfo.size(); n++)
        {
            file.PushList(std::to_string(n));
            mHitInfo[n].SaveToKeyValueFile(file);
            file.PopList();
        }
        file.PopList();
    }
}

// ======================================
void AttackComponent::ShowImguiDetails() 
{
    // TODO: Ability to play an attack in editor for testing
    // TODO: Ability to change attack name
    if (ImGui::Button("Add attack")) {
        mAttackMap.insert({"NEW ATTACK CHAIN (change name in file)", {}});
    }
    for (auto& it : mAttackMap)
    {
        const std::string& name = it.first;
        if (ImGui::TreeNode(name.empty() ? "unnamed" : name.c_str()))
        {
            if (ImGui::Button("Add attack combo"))
            {
                it.second.emplace_back();
            }
            for (int n = 0; n < it.second.size(); ++n)
            {
                if (ImGui::TreeNode(std::to_string(n).c_str()))
                {
                    it.second[n].ShowImguiDetails();
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
}

// ======================================
void AttackComponent::SaveToKeyValueFile(Shade::KeyValueFile& file) const
{
    for (auto& it : mAttackMap)
    {
        file.PushList(it.first);
        for (int n = 0; n < it.second.size(); n++)
        {
            file.PushList(std::to_string(n));
            it.second[n].SaveToKeyValueFile(file);
            file.PopList();
        }
        file.PopList();
    }
}
#endif

// ======================================
AttackHitInfo AttackHitInfo::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    AttackHitInfo hitInfo;
    while (handle.IsValid())
    {
        if (handle.GetKey() == "anim_frame")
        {
            hitInfo.mTriggerFrame = handle.TryGetInt(hitInfo.mTriggerFrame);
        }
        if (handle.GetKey() == "damage")
        {
            hitInfo.mDamage = handle.TryGetFloat(hitInfo.mDamage);
        }
        if (handle.GetKey() == "target")
        {
            hitInfo.mTarget = static_cast<AttackTarget>(handle.TryGetInt(static_cast<int>(hitInfo.mTarget)));
        }
        if (handle.GetKey() == "boxes")
        {
            Shade::KeyValueHandle boxesHandle = handle.GetListHead();
            while (boxesHandle.IsValid())
            {
                Shade::KeyValueHandle boxHandle = boxesHandle.GetListHead();
                AttackHitBox attackHitBox;
                while (boxHandle.IsValid())
                {
                    if (boxHandle.GetKey() == "width")
                    {
                        attackHitBox.mWidth = boxHandle.TryGetFloat();
                    }
                    if (boxHandle.GetKey() == "height")
                    {
                        attackHitBox.mHeight = boxHandle.TryGetFloat();
                    }
                    if (boxHandle.GetKey() == "offset_x")
                    {
                        attackHitBox.mOffsetX = boxHandle.TryGetFloat();
                    }
                    if (boxHandle.GetKey() == "offset_y")
                    {
                        attackHitBox.mOffsetY = boxHandle.TryGetFloat();
                    }
                    if (boxHandle.GetKey() == "effect")
                    {
                        Shade::KeyValueHandle effectHandle = boxHandle.GetListHead();
                        while (effectHandle.IsValid())
                        {
                            if (effectHandle.GetKey() == "path")
                            {
                                // TODO: Consider whether we actually have to load the effect here
                                //  - Perhaps just storing/working with the string would be sufficient
                                const std::string& effectPath = effectHandle.TryGetString();
                                #ifdef BUILD_BREACH_EDITOR
                                attackHitBox.mEffectPath = effectPath;
                                #endif
                                Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
                                Shade::ResourceHandle fxHandle = resourceManager->LoadResource<Shade::Texture>(effectPath);
                                if (!fxHandle.IsValid())
                                {
                                    Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
                                    logService->LogWarning(std::string("vfx for attack failed to load: ") + effectPath);
                                    continue;
                                }
                                attackHitBox.mEffectTexture = fxHandle;
                            }
                            if (effectHandle.GetKey() == "effect_offset_x")
                            {
                                attackHitBox.mEffectOffsetX = effectHandle.TryGetFloat(attackHitBox.mEffectOffsetX);
                            }
                            if (effectHandle.GetKey() == "effect_offset_y")
                            {
                                attackHitBox.mEffectOffsetY = effectHandle.TryGetFloat(attackHitBox.mEffectOffsetY);
                            }
                            effectHandle.ToNext();
                        }
                    }
                    boxHandle.ToNext();
                }
                hitInfo.mAttackBoxes.emplace_back(attackHitBox);
                boxesHandle.ToNext();
            }
        }
        handle.ToNext();
    }
    return hitInfo;
}

// ======================================
AttackComponent* AttackComponent::LoadFromFileHandle(Shade::KeyValueHandle handle)
{
    Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    AttackComponent* attackComponent = new AttackComponent();

    while (handle.IsValid())
    {
        const std::string& name = handle.GetKey();
        if (!handle.IsList())
        {
            logService->LogError(std::string("Error parsing attack component, expected list format for attack details: ") + name);
        }

        Shade::KeyValueHandle attackHandle = handle.GetListHead();
        while (attackHandle.IsValid())
        {
            AttackInfo attackInfo;
            Shade::KeyValueHandle comboAttackHAndle = attackHandle.GetListHead();
            while (comboAttackHAndle.IsValid())
            {
                if (comboAttackHAndle.GetKey() == "anim")
                {
                    attackInfo.mAnimation = comboAttackHAndle.TryGetString(attackInfo.mAnimation);
                }
                if (comboAttackHAndle.GetKey() == "disable_movement")
                {
                    attackInfo.mDisableMovement = static_cast<bool>(comboAttackHAndle.TryGetInt(static_cast<int>(attackInfo.mDisableMovement)));
                }
                if (comboAttackHAndle.GetKey() == "invulnerable")
                {
                    attackInfo.mInvulnerable = static_cast<bool>(comboAttackHAndle.TryGetInt(static_cast<int>(attackInfo.mInvulnerable)));
                }
                if (comboAttackHAndle.GetKey() == "duration")
                {
                    attackInfo.mDuration = comboAttackHAndle.TryGetFloat(attackInfo.mDuration);
                }
                if (comboAttackHAndle.GetKey() == "speed")
                {
                    attackInfo.mMoveSpeed = comboAttackHAndle.TryGetFloat(attackInfo.mMoveSpeed);
                }
                if (comboAttackHAndle.GetKey() == "hits")
                {
                    assert(comboAttackHAndle.IsList() && "Attack handle should be list format");
                    Shade::KeyValueHandle hitsHandle = comboAttackHAndle.GetListHead();
                    while (hitsHandle.IsValid())
                    {
                        Shade::KeyValueHandle hitHandle = hitsHandle.GetListHead();
                        attackInfo.mHitInfo.emplace_back(AttackHitInfo::LoadFromFileHandle(hitHandle));
                        hitsHandle.ToNext();
                    }
                }   
                comboAttackHAndle.ToNext();
            }
            attackHandle.ToNext();
            attackComponent->RegisterAttackInfo(name, attackInfo);
        }
        
        handle.ToNext();
    }

    return attackComponent;
}

// ======================================
AttackComponent::AttackComponent()
    : mAttackMap()
{
    
}

// ======================================
AttackInfo& AttackComponent::RegisterAttackInfo(const std::string& name, const AttackInfo& attack)
{
    if (mAttackMap.find(name) != mAttackMap.end())
    {
        mAttackMap[name].emplace_back(attack);
    }
    else
    {
        mAttackMap[name] = { attack };
    }
    return mAttackMap[name].back();
}

// ======================================
void AttackComponent::RegisterAttacksToAnimFrames()
{
    Shade::AnimatedSpriteComponent* anim = mEntityRef->GetCachedAnimatedSprite();
    if (anim == nullptr)
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogWarning("Tried to register attacks to entity without animated sprite component");
        return;
    }
    for (const auto& pair : mAttackMap)
    {
        for (const AttackInfo& attack : pair.second)
        {
            for (const AttackHitInfo& attackHitInfo : attack.mHitInfo)
        {
            const uint32_t attackHitFrame = attackHitInfo.mTriggerFrame;
            anim->mEvents[attackHitFrame] = [this, attackHitInfo](Shade::Entity* triggerEntity) {
                this->TriggerAttackHitEvent(attackHitInfo);
            };
        }
        }
    }
}

// ======================================
void AttackComponent::Initialize()
{
    RegisterAttacksToAnimFrames();
}

// ======================================
void AttackComponent::Update(float deltaSeconds)
{
    if (!mCurrentAttack.empty())
    {
        assert(mComboWindow <= 0.f && "Combo window should not be active while an attack is executing");
        assert(mAttackMap.find(mCurrentAttack) != mAttackMap.end() && "Current attack was not found in list of attacks");
        const AttackInfo& attackInfo = mAttackMap[mCurrentAttack][mCurrentAttackIndex];
        if (attackInfo.mMoveSpeed > 0.f)
        {
            BaseMovementComponent* movement = mEntityRef->GetComponent<BaseMovementComponent>();
            if (mCurrentAttackFacing == FacingDirection::LEFT)
            {
                movement->MoveLeft(attackInfo.mMoveSpeed * deltaSeconds);
            }
            if (mCurrentAttackFacing == FacingDirection::RIGHT)
            {
                movement->MoveRight(attackInfo.mMoveSpeed * deltaSeconds);
            }
        }
        mCurrentAttackTimer -= deltaSeconds;
        if (mCurrentAttackTimer <= 0.f)
        {
            // TODO: Combo window timing should maybe not be hard-coded?
            mComboWindow = 0.2f;
            mLastAttack = mCurrentAttack;

            mCurrentAttack.clear();
            mCurrentAttackTimer = 0.f;
            if (attackInfo.mDisableMovement)
            {
                LocomotionComponent* locomotion = mEntityRef->GetComponent<LocomotionComponent>();
                locomotion->EnableLocomotion();
            }
            if (attackInfo.mInvulnerable)
            {
                HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
                health->mIsInvulnerable = false;
            }
            // TODO: Not all attacks will disable stagger - need to set in attack
            if (StaggerComponent* stagger = mEntityRef->GetComponent<StaggerComponent>())
            {
                stagger->EnableStagger();
            }
        }
    }
    if (mComboWindow > 0.f)
    {
        mComboWindow -= deltaSeconds;
        if (mComboWindow <= 0.f)
        {
            mLastAttack.clear();
            mComboWindow = 0.f;
            mCurrentAttackIndex = 0;
        }
    }
}

// ======================================
bool AttackComponent::IsDoingAttack() const
{
    return !mCurrentAttack.empty();
}

// ======================================
bool AttackComponent::TryDoAttack(const std::string& name)
{
    if (IsDoingAttack())
    {
        return false;
    }
    return DoAttack(name);
}

// ======================================
void AttackComponent::TriggerAttackHitEvent(const AttackHitInfo& attackInfo)
{
    for (const AttackHitBox& attackHitBox : attackInfo.mAttackBoxes)
    {
        Shade::Box attackBox = Shade::Box(Shade::Vec2{ mEntityRef->GetPositionX() + attackHitBox.mOffsetX, mEntityRef->GetPositionY() + attackHitBox.mOffsetY}, attackHitBox.mWidth, attackHitBox.mHeight);

        // Spawn vfx if specified by the attack
        if (attackHitBox.mEffectTexture.IsValid())
        {
            Shade::FlareService* flareVFX = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FlareService>();
            Shade::Vec2 vfxPosition = attackBox.mPosition;
            vfxPosition.x += attackHitBox.mEffectOffsetX;
            vfxPosition.y += attackHitBox.mEffectOffsetY;
            // TODO: Adjustable render layers?
            // TODO: Adjustable vfx lifetime
            // TODO: Don't want to manually specify width/height here
            flareVFX->SpawnEffect(Shade::Effect{ vfxPosition, 80.f, 140.f, attackHitBox.mEffectTexture, static_cast<int>(RenderLayer::DEFAULT), 0.25f});
        }
        // Check the actual hit box for hits on enemies
        for (const auto& entity : mEntityRef->GetWorldEntities())
        {
            // The attack has already hit the entity, don't hit it twice
            if (std::find(mCurrentHitEnemies.begin(), mCurrentHitEnemies.end(), entity.get()) != mCurrentHitEnemies.end())
            {
                continue;
            }

            HealthComponent* health = entity->GetComponent<HealthComponent>();
            if (health && health->IsDead())
            {
                continue;
            }
            if (entity.get() == mEntityRef)
            {
                continue;
            }
            // TODO: Need an ACTUAL way to determine if an entity is a player or enemy
            if (attackInfo.mTarget == AttackTarget::PLAYER && entity->GetComponent<StateMachineAIComponent>() != nullptr)
            {
                continue;
            }
            if (attackInfo.mTarget == AttackTarget::ENEMY && entity->GetComponent<StateMachineAIComponent>() == nullptr)
            {
                continue;
            }
            if (HitboxComponent* hitbox = entity->GetComponent<HitboxComponent>())
            {
                if (hitbox->Intersects(attackBox))
                {
                    if (health != nullptr)
                    {
                        health->DecrementHealth(attackInfo.mDamage);

                        // TODO: More thought needs to be put into the stagger design
                        //  - Certain enemies may not want to be staggered (bosses, gigantic enemies, etc)
                        //      - Boss staggers can be implemented as a short timed delay rather than full animation
                        //  - A "weight class" system can be used to determine stagger times/triggers
                        //  - Some attacks also may not want to stagger enemies or do so at different times (light attacks)
                        //  - Stagger direction also needs to be accounted for here
                        if (StaggerComponent* stagger = entity->GetComponent<StaggerComponent>())
                        {
                            // TODO: Replace hard-coded number - embed into attack data
                            stagger->TryStaggerInDirection(0.5f, mCurrentAttackFacing == FacingDirection::RIGHT ? FacingDirection::LEFT : FacingDirection::RIGHT);
                        }
                    }
                    
                    mCurrentHitEnemies.emplace_back(entity.get());

                    // TODO: This should be data-driven by the attack
                    // TODO: Better calculation of where the vfx should play
                    Shade::Vec2 vfxPosition = attackBox.mPosition;
                    vfxPosition.x += attackBox.mWidth / 2.f - (mCurrentAttackFacing == FacingDirection::RIGHT ? 0.f : 40.f);
                    vfxPosition.y += attackBox.mHeight / 2.f;
                    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
                    Shade::ResourceHandle hardCodedHandle  = resourceManager->LoadResource<Shade::Texture>(mCurrentAttackFacing == FacingDirection::RIGHT ? "assets/breach/VFX/whack_right.png" : "assets/breach/VFX/whack_left.png");
                    Shade::FlareService* flareVFX = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FlareService>();
                    flareVFX->SpawnEffect(Shade::Effect{ vfxPosition, 40.f, 30.f, hardCodedHandle, static_cast<int>(RenderLayer::VFX_TOP), 0.25f});
                }
            }
        }
#ifdef DEBUG_BREACH
        DebugUtils::DrawDebugRectOutline(Shade::Vec2{ mEntityRef->GetPositionX() + attackHitBox.mOffsetX, mEntityRef->GetPositionY() + attackHitBox.mOffsetY}, attackHitBox.mWidth, attackHitBox.mHeight, Shade::Colour{ 0.15f, 0.15f, 0.15f}, 0.5f);
#endif
    }
}

// ======================================
bool AttackComponent::DoAttack(const std::string& name)
{
    auto it = mAttackMap.find(name);
    if (it == mAttackMap.end())
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogWarning("Tried to do an attack that doesn't exist: " + name);
        return false;
    }
    FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
    if (!facing)
    {
        Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logService->LogError("Missing facing component on entity with attack component");
        return false;
    }

    // TODO: Combo stuff should be handled one level higher, this function should just cover actually executing a specific combo attack
    if (mComboWindow > 0.f)
    {
        mComboWindow = 0.f;
        if (mLastAttack == name)
        {
            mCurrentAttackIndex++;
        }
        mLastAttack.clear();
    }
    if (mCurrentAttackIndex >= it->second.size())
    {
        mCurrentAttackIndex = 0;
    }

    const AttackInfo& attackInfo = it->second[mCurrentAttackIndex];
    mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(attackInfo.mAnimation);
    if (attackInfo.mDisableMovement)
    {
        LocomotionComponent* locomotion = mEntityRef->GetComponent<LocomotionComponent>();
        locomotion->DisableLocomotion();
    }
    if (attackInfo.mInvulnerable)
    {
        HealthComponent* health = mEntityRef->GetComponent<HealthComponent>();
        health->mIsInvulnerable = true;
    }
    mCurrentAttack = name;
    mCurrentHitEnemies.clear();
    mCurrentAttackTimer = attackInfo.mDuration;
    mCurrentAttackFacing = facing->mDirection;

    // TODO: Not all attacks will disable stagger - need to set in attack`
    if (StaggerComponent* stagger = mEntityRef->GetComponent<StaggerComponent>())
    {
        stagger->DisableStagger();
    }

    return true;
}