#include "entity.h"

#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"

// ======================================
Shade::Entity::Entity() = default;

// ======================================
Shade::Entity::~Entity() = default;

// ======================================
void Shade::Entity::Update(float DeltaSeconds)
{
    for (std::unique_ptr<Component>& CurrentComponent : mComponents)
    {
        CurrentComponent->Update(DeltaSeconds);
    }
    if (mCachedSprite.has_value())
    {
        mCachedSprite->get()->Update(DeltaSeconds);
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
// This takes ownership of NewComponent
void Shade::Entity::AddComponent(std::unique_ptr<Component> NewComponent)
{
    if (SpriteComponent* Sprite = dynamic_cast<SpriteComponent*>(NewComponent.get()))
    {
        NewComponent.release();
        mCachedSprite = std::unique_ptr<SpriteComponent>(Sprite);
    }
    else
    {
        mComponents.emplace_back(std::move(NewComponent));
    }
}

// ======================================
Shade::SpriteComponent* Shade::Entity::GetCachedSpriteComponent() const
{
    return mCachedSprite.has_value() ? mCachedSprite->get() : nullptr;
}