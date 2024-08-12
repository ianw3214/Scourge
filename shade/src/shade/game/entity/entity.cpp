#include "entity.h"

#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/container.h"
#include "shade/game/event/source.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/logging/logger.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#ifdef BUILD_SHADE_EDITOR
// ======================================
void Shade::Entity::ShowImguiDetails()
{
    if (ImGui::TreeNode("Sprite details"))
    {
        if (mCachedSprite.has_value())
        {
            mCachedSprite->get()->ShowImguiDetails();
        }
        else
        {
            ImGui::Text("No sprite component set...");
            if (ImGui::Button("Add sprite component"))
            {
                AddComponent(std::make_unique<SpriteComponent>());
            }
            if (ImGui::Button("Add animated sprite component"))
            {
                AddComponent(std::make_unique<AnimatedSpriteComponent>());
            }
        }
        ImGui::TreePop();
    }
    for (std::unique_ptr<Component>& component : mComponents)
    {
        if (ImGui::TreeNode(component->GetDisplayName()))
        {
            component->ShowImguiDetails();
            ImGui::TreePop();
        }
    }
}
#endif

// ======================================
Shade::Entity::Entity(GameplayEventSource& gameWorldRef, EntityContainer& entityContainer) 
    : mGameEventSource(gameWorldRef)
    , mEntityContainer(entityContainer) 
{

}

// ======================================
Shade::Entity::~Entity() = default;

// ======================================
void Shade::Entity::Update(float deltaSeconds)
{
    for (std::unique_ptr<Component>& component : mComponents)
    {
        component->Update(deltaSeconds);
    }
    if (mCachedSprite.has_value())
    {
        mCachedSprite->get()->Update(deltaSeconds);
    }
}

// ======================================
void Shade::Entity::MarkForDelete()
{
    mMarkDelete = true;
}

// ======================================
bool Shade::Entity::IsMarkedForDelete() const
{
    return mMarkDelete;
}

// ======================================
void Shade::Entity::SetPositionX(float newX)
{
    x = newX;
}

// ======================================
void Shade::Entity::SetPositionY(float newY)
{
    y = newY;
}

// ======================================
void Shade::Entity::SetName(const std::string& name)
{
    mName = name;
}

// ======================================
float Shade::Entity::GetPositionX() const
{
    return x;
}

// ======================================
float Shade::Entity::GetPositionY() const
{
    return y;
}

// ======================================
Shade::Vec2 Shade::Entity::GetPosition() const
{
    return Shade::Vec2{ x, y };
}

// ======================================
const std::string& Shade::Entity::GetName() const
{
    return mName;
}

// ======================================
// This takes ownership of newComponent
void Shade::Entity::AddComponent(std::unique_ptr<Component> newComponent)
{
    newComponent->SetEntityRef(this);
    if (SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(newComponent.get()))
    {
        if (mCachedSprite)
        {
            LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
            logService->LogError("Sprite component already exists on entity: {0}");
            return;
        }
        newComponent.release();
        mCachedSprite = std::unique_ptr<SpriteComponent>(sprite);
    }
    else
    {
        mComponents.emplace_back(std::move(newComponent));
    }
}

// ======================================
Shade::SpriteComponent* Shade::Entity::GetCachedSpriteComponent() const
{
    return mCachedSprite.has_value() ? mCachedSprite->get() : nullptr;
}

// ======================================
Shade::AnimatedSpriteComponent* Shade::Entity::GetCachedAnimatedSprite() const
{
    return mCachedSprite.has_value() ? dynamic_cast<AnimatedSpriteComponent*>(mCachedSprite->get()) : nullptr;
}

// ======================================
const Shade::BooleanGameplayEvent& Shade::Entity::GetBooleanEvent(const std::string& gameEvent) const
{
    return mGameEventSource.GetBooleanEvent(gameEvent);
}

// ======================================
const Shade::IntGameplayEvent& Shade::Entity::GetIntEvent(const std::string& gameEvent) const
{
    return mGameEventSource.GetIntEvent(gameEvent);
}

// ======================================
const Shade::FloatGameplayEvent& Shade::Entity::GetFloatEvent(const std::string& gameEvent) const
{
    return mGameEventSource.GetFloatEvent(gameEvent);
}

// ======================================
std::vector<std::unique_ptr<Shade::Entity>>& Shade::Entity::GetWorldEntities() const
{
    return mEntityContainer.GetEntities();
}