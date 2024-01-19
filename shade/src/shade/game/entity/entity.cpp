#include "entity.h"

#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/event/source.h"

// ======================================
Shade::Entity::Entity(GameplayEventSource& GameWorldRef) 
    : mGameEventSource(GameWorldRef) 
{

}

// ======================================
Shade::Entity::~Entity() = default;

// ======================================
void Shade::Entity::Update(float deltaSeconds)
{
    for (std::unique_ptr<Component>& CurrentComponent : mComponents)
    {
        CurrentComponent->Update(deltaSeconds);
    }
    if (mCachedSprite.has_value())
    {
        mCachedSprite->get()->Update(deltaSeconds);
    }
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
// This takes ownership of newComponent
void Shade::Entity::AddComponent(std::unique_ptr<Component> newComponent)
{
    // TODO: Does it make sense to check if a sprite component already exists here?
    if (SpriteComponent* sprite = dynamic_cast<SpriteComponent*>(newComponent.get()))
    {
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