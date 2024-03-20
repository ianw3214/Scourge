#pragma once

#include <vector>
#include <memory>
#include <string>

#include "shade/common/vec.h"
#include "shade/common/colour.h"
#include "shade/graphics/common.h"
#include "shade/resource/handle.h"

namespace Shade {

    class RenderCommand;

    class RendererBase {
    public:
        RendererBase();
        ~RendererBase();

        void InitializeDefaultShaders();

        void Clear();
        void SetClearColor(float r, float g, float b, float a);
        void SetWireframeMode(bool wireframeMode = true);

        void QueueCommand(std::unique_ptr<RenderCommand> command);
        void ProcessCommandQueue();
        void SwapCommandQueue(std::vector<std::unique_ptr<RenderCommand>>& newCommandQueue);

        // Should pixel coordinates be integers?
        void DrawLine(float point1x, float point1y, float point2x, float point2y, Colour colour, float depth = -1.f) const;
        void DrawLine(Vec2 point1, Vec2 point2, Colour colour, float depth = -1.f) const;
        void DrawLineNormalized(float point1x, float point1y, float point2x, float point2y, Colour colour, float depth = -1.f) const;
        void DrawLineNormalized(Vec2 point1, Vec2 point2, Colour colour, float depth = -1.f) const;
        void DrawRectangle(float x, float y, float w, float h, Colour colour, float depth = -1.f) const;
        void DrawRectangle(Vec2 pos, float w, float h, Colour colour, float depth = -1.f) const;
        void DrawRectangleNormalized(float x, float y, float w, float h, Colour colour, float depth = -1.f) const;
        void DrawRectangleNormalized(Vec2 pos, float w, float h, Colour colour, float depth = -1.f) const;
        // These string will eventually become texture resource handles in the engine
        void DrawTexture(float x, float y, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth = 1.f) const;
        void DrawTexture(Vec2 pos, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth = 1.f) const;
        void DrawTextureNormalized(float x, float y, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth = 1.f) const;
        void DrawTextureNormalized(Vec2 pos, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth = 1.f) const;
    private:
        // Food for thought - does this need to be a unique_ptr?
        //  Can this be turned into a struct w/o inheritance
        std::vector<std::unique_ptr<RenderCommand>> mCommandQueue;
    };

}