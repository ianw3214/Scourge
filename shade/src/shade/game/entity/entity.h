#pragma once

#include <memory>
#include <vector>
#include <optional>

namespace Shade {

    class Component;
    class SpriteComponent;
    class Entity {
    public:
        Entity();
        ~Entity();

        void Update(float DeltaSeconds);

        void SetPositionX(float newX);
        void SetPositionY(float newY);
        float GetPositionX() const;
        float GetPositionY() const;

        void AddComponent(std::unique_ptr<Component> NewComponent);
        SpriteComponent* GetCachedSpriteComponent() const;
    private:
        // Default entity attributes
        float x = 0.f;
        float y = 0.f;

        // The components list does NOT contain cached components
        std::vector<std::unique_ptr<Component>> mComponents;
        // Cached components are updated last
        std::optional<std::unique_ptr<SpriteComponent>> mCachedSprite;
    };

}