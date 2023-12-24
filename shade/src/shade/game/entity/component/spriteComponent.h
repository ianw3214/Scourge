#pragma once

#include <memory>
#include <string>

#include "shade/game/entity/component/component.h"
#include "shade/resource/handle.h"

namespace Shade {

    class AnimDefinition {
    public:
        
    private:
    };

    class DrawTextureCommand;
    class SpriteComponent : public Component {
    public:
        SpriteComponent(Entity& Owner);
        SpriteComponent(Entity& Owner, float RenderWidth, float RenderHeight, std::string TexturePath);

        float GetRenderWidth() const;
        float GetRenderHeight() const;

        std::unique_ptr<DrawTextureCommand> CreateRenderCommand();
    private:
        float mRenderWidth = 0.f;
        float mRenderHeight = 0.f;
        ResourceHandle mTextureHandle = ResourceHandle::Invalid;
    };

}