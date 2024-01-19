#pragma once

#include <memory>
#include <string>

#include "shade/game/entity/component/component.h"
#include "shade/resource/handle.h"

namespace Shade {

    class DrawTextureCommand;
    class SpriteComponent : public Component {
    public:
        SpriteComponent(Entity& owner);
        SpriteComponent(Entity& owner, float renderWidth, float renderHeight, std::string texturePath);

        float GetrenderWidth() const;
        float GetrenderHeight() const;

        virtual std::unique_ptr<DrawTextureCommand> CreateRenderCommand();
    protected:
        float mrenderWidth = 0.f;
        float mrenderHeight = 0.f;
        ResourceHandle mTextureHandle = ResourceHandle::Invalid;
    };

}