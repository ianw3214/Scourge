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
        SpriteComponent(Entity& owner, float renderWidth, float renderHeight, std::string texturePath, int renderLayer = 0);

        float GetrenderWidth() const;
        float GetrenderHeight() const;

        virtual std::unique_ptr<DrawTextureCommand> CreateRenderCommand();
    protected:
        float mRenderWidth = 0.f;
        float mRenderHeight = 0.f;
        int mRenderLayer = 0;
        ResourceHandle mTextureHandle = ResourceHandle::Invalid;
    };

}