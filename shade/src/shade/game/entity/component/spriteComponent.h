#pragma once

#include <memory>
#include <string>

#include "shade/game/entity/component/component.h"
#include "shade/resource/handle.h"

namespace Shade {

    // Defines which point of the sprite counts as the position of the sprite
    //  - might not be needed once custom offsets are implemented
    enum class RenderAnchor {
        BOTTOM_LEFT,
        BOTTOM_MIDDLE,
        MIDDLE
    };

    class DrawTextureCommand;
    class SpriteComponent : public Component {
    public:
        SpriteComponent(std::string texturePath, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE);
        SpriteComponent(float renderWidth, float renderHeight, std::string texturePath, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE);

        float GetRenderWidth() const;
        float GetRenderHeight() const;
        // These take into account the rendering offsets of a sprite based on the entity position
        float GetRenderX() const;
        float GetRenderY() const;

        virtual std::unique_ptr<DrawTextureCommand> CreateRenderCommand();
    protected:
        float mRenderWidth = 0.f;
        float mRenderHeight = 0.f;
        RenderAnchor mRenderAnchor = RenderAnchor::MIDDLE;
        int mRenderLayer = 0;
        ResourceHandle mTextureHandle = ResourceHandle::Invalid;
    };

}