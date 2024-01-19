#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <string>

namespace Shade {

    struct BooleanGameplayEvent;
    class AnimatedSpriteComponent;
    class Component;
    class GameplayEventSource;
    class SpriteComponent;

    class Entity {
    public:
        Entity(GameplayEventSource&);
        ~Entity();

        void Update(float deltaSeconds);

        void SetPositionX(float newX);
        void SetPositionY(float newY);
        float GetPositionX() const;
        float GetPositionY() const;

        void AddComponent(std::unique_ptr<Component> newComponent);
        SpriteComponent* GetCachedSpriteComponent() const;
        AnimatedSpriteComponent* GetCachedAnimatedSprite() const;

        const BooleanGameplayEvent& GetBooleanEvent(const std::string& gameEvent) const;
    private:
        // Default entity attributes
        float x = 0.f;
        float y = 0.f;

        // The components list does NOT contain cached components
        std::vector<std::unique_ptr<Component>> mComponents;
        // Cached components are updated last
        std::optional<std::unique_ptr<SpriteComponent>> mCachedSprite;

        // Store a reference to the game world
        GameplayEventSource& mGameEventSource;
    };

}