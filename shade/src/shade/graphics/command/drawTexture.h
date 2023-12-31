#pragma once

#include <string>

#include "shade/graphics/command/command.h"
#include "shade/graphics/renderer.h"
#include "shade/resource/handle.h"

#include "shade/common/vec.h"

namespace Shade {

    class DrawTextureCommand : public RenderCommand {
    public:
        DrawTextureCommand();
        DrawTextureCommand(Vec2 Position, float Width, float Height);
        DrawTextureCommand(Vec2 Position, float Width, float Height, ResourceHandle TextureResource);
        DrawTextureCommand(Vec2 Position, float Width, float Height, ResourceHandle TextureResource, bool Normalized);
        DrawTextureCommand(Vec2 Position, float Width, float Height, ResourceHandle TextureResource, bool Normalized, TextureSourceInfo TextureSource);
        DrawTextureCommand(float xPosition, float yPosition, float Width, float Height);
        DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, ResourceHandle TextureResource);
        DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, ResourceHandle TextureResource, bool Normalized);
        DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, ResourceHandle TextureResource, bool Normalized, TextureSourceInfo TextureSource);

        virtual void Execute(RendererBase* Renderer) override;
    private:
            Vec2 mPosition;
            float mWidth = 1.f;
            float mHeight = 1.f;
            ResourceHandle mResourceHandle = ResourceHandle::Invalid;
            bool mNormalized = false;
            TextureSourceInfo mTextureSource = {};
    };

}