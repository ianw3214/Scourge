#pragma once

#include <vector>
#include <memory>
#include <string>

#include "shade/common/vec.h"
#include "shade/common/colour.h"

namespace Shade {

    class RenderCommand;

    class RendererBase {
    public:
        ~RendererBase();

        void InitializeDefaultShaders();

        void Clear();
        void SetClearColor(float r, float g, float b, float a);
        void SetWireframeMode(bool wireframeMode = true);

        void QueueCommand(std::unique_ptr<RenderCommand> Command);
        void ProcessCommandQueue();
        void SwapCommandQueue(std::vector<std::unique_ptr<RenderCommand>>& NewCommandQueue);

        // Should pixel coordinates be integers?
        void DrawLine(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour) const;
        void DrawLine(Vec2 Point1, Vec2 Point2, Colour LineColour) const;
        void DrawLineNormalized(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour) const;
        void DrawLineNormalized(Vec2 Point1, Vec2 Point2, Colour LineColour) const;
        void DrawRectangle(float x, float y, float w, float h, Colour RectColour) const;
        void DrawRectangle(Vec2 pos, float w, float h, Colour RectColour) const;
        void DrawRectangleNormalized(float x, float y, float w, float h, Colour RectColour) const;
        void DrawRectangleNormalized(Vec2 pos, float w, float h, Colour RectColour) const;
        // These string will eventually become texture resource handles in the engine
        void DrawTexture(float x, float y, float w, float h, const std::string& TexturePath) const;
        void DrawTexture(Vec2 pos, float w, float h, const std::string& TexturePath) const;
        void DrawTextureNormalized(float x, float y, float w, float h, const std::string& TexturePath) const;
        void DrawTextureNormalized(Vec2 pos, float w, float h, const std::string& TexturePath) const;
    private:
        // Food for thought - does this need to be a unique_ptr?
        //  Can this be turned into a struct w/o inheritance
        std::vector<std::unique_ptr<RenderCommand>> mCommandQueue;
    };

}