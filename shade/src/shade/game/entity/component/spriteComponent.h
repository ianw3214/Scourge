#pragma once

#include <memory>
#include <string>
#include <vector>

#include "shade/game/entity/component/component.h"
#include "shade/graphics/common.h"
#include "shade/resource/handle.h"
#include "shade/common/colour.h"

namespace Shade {

    class RenderCommand;
    class SpriteComponent : public Component {
    public:
        SpriteComponent(std::string texturePath, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE, bool constantDepth = false);
        SpriteComponent(float renderWidth, float renderHeight, std::string texturePath, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE, bool constantDepth = false);

        float GetRenderWidth() const;
        float GetRenderHeight() const;
        // These take into account the rendering offsets of a sprite based on the entity position
        float GetRenderX() const;
        float GetRenderY() const;

        void SetColourMultiplier(Colour colour);
        void ResetColourMultiplier();

        virtual void AddRenderCommands(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue);
    protected:
        float mRenderWidth = 0.f;
        float mRenderHeight = 0.f;
        //  Render anchor might not be needed if  custom offsets are implemented
        RenderAnchor mRenderAnchor = RenderAnchor::MIDDLE;
        int mRenderLayer = 0;
        bool mConstantDepth = false;
        ResourceHandle mTextureHandle = ResourceHandle::Invalid;
        Colour mColourMultiplier = { 1.f, 1.f, 1.f };
    };

}