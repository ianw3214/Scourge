#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <string>

#include "shade/common/vec.h"

namespace Shade {

    struct BooleanGameplayEvent;
    struct IntGameplayEvent;
    struct FloatGameplayEvent;
    class AnimatedSpriteComponent;
    class Component;
    class EntityContainer;
    class GameplayEventSource;
    class SpriteComponent;

    class Entity {
    public:
        Entity(GameplayEventSource&, EntityContainer&);
        ~Entity();

        void Update(float deltaSeconds);

        void MarkForDelete();
        bool IsMarkedForDelete() const;

        void SetPositionX(float newX);
        void SetPositionY(float newY);
        float GetPositionX() const;
        float GetPositionY() const;
        Vec2 GetPosition() const;

        void AddComponent(std::unique_ptr<Component> newComponent);
        template<class ComponentClass> ComponentClass* GetComponent();
        SpriteComponent* GetCachedSpriteComponent() const;
        AnimatedSpriteComponent* GetCachedAnimatedSprite() const;

        const BooleanGameplayEvent& GetBooleanEvent(const std::string& gameEvent) const;
        const IntGameplayEvent& GetIntEvent(const std::string& gameEvent) const;
        const FloatGameplayEvent& GetFloatEvent(const std::string& gameEvent) const;

        // TODO: This feels weird to keep in the entity base class
        //  - Consider where else the entity container can be referenced by
        //  - Perhaps this needs to just be implemented as a globally accessible service
        std::vector<std::unique_ptr<Entity>>& GetWorldEntities() const;
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
        EntityContainer& mEntityContainer;

        bool mMarkDelete = false;
    };

}

namespace Shade {

    template<class ComponentClass>
    ComponentClass* Entity::GetComponent()
    {
        for (const std::unique_ptr<Component>& component : mComponents)
        {
            ComponentClass* derivedComponent = dynamic_cast<ComponentClass*>(component.get());
            if (derivedComponent != nullptr)
            {
                return derivedComponent;
            }
        }
        return nullptr;
    }

}