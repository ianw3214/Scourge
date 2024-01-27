#pragma once

#include <string>

#include "shade/graphics/command/command.h"
#include "shade/graphics/common.h"
#include "shade/resource/handle.h"

#include "shade/common/vec.h"

namespace Shade {

    // TODO: Use builder pattern to avoid constructor hell
    class DrawTextureCommand : public RenderCommand {
    public:
        DrawTextureCommand();
        DrawTextureCommand(Vec2 position, float width, float height);
        DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource);
        DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource, int layer);
        DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource, bool normalized);
        DrawTextureCommand(Vec2 position, float width, float height, ResourceHandle textureResource, bool normalized, textureSourceInfo textureSource);
        DrawTextureCommand(float xPosition, float yPosition, float width, float height);
        DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource);
        DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, int layer);
        DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, bool normalized);
        DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, bool normalized, textureSourceInfo textureSource);
        DrawTextureCommand(float xPosition, float yPosition, float width, float height, ResourceHandle textureResource, bool normalized, textureSourceInfo textureSource, int layer);

        virtual void Execute(RendererBase* renderer, const CameraInfo& camera) override;
    private:
            Vec2 mPosition;
            float mWidth = 1.f;
            float mHeight = 1.f;
            int mLayer = 0;
            ResourceHandle mResourceHandle = ResourceHandle::Invalid;
            bool mNormalized = false;
            textureSourceInfo mTextureSource = {};
    };

}