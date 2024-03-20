#pragma once

#include "shade/graphics/command/command.h"

#include "shade/common/vec.h"
#include "shade/common/colour.h"

namespace Shade {

    class DrawRectangleCommand : public RenderCommand {
    public:
        DrawRectangleCommand();
        DrawRectangleCommand(Vec2 position, float width, float height);
        DrawRectangleCommand(Vec2 position, float width, float height, Colour colour);
        DrawRectangleCommand(Vec2 position, float width, float height, Colour colour, bool normalized);
        DrawRectangleCommand(float xPosition, float yPosition, float width, float height);
        DrawRectangleCommand(float xPosition, float yPosition, float width, float height, Colour colour);
        DrawRectangleCommand(float xPosition, float yPosition, float width, float height, Colour colour, bool normalized);

        virtual void Execute(RendererBase* Renderer, const CameraInfo& camera) override;
    private:
            Vec2 mPosition;
            float mWidth = 1.f;
            float mHeight = 1.f;
            Colour mColour;
            bool mNormalized = false;
            // TODO: Filled option
    };

}