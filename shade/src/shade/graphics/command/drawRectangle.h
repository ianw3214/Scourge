#pragma once

#include "shade/graphics/command/command.h"

#include "shade/common/vec.h"
#include "shade/common/colour.h"

namespace Shade {

    class DrawRectangleCommand : public RenderCommand {
    public:
        DrawRectangleCommand();
        DrawRectangleCommand(Vec2 Position, float Width, float Height);
        DrawRectangleCommand(Vec2 Position, float Width, float Height, Colour RectColour);
        DrawRectangleCommand(Vec2 Position, float Width, float Height, Colour RectColour, bool Normalized);
        DrawRectangleCommand(float xPosition, float yPosition, float Width, float Height);
        DrawRectangleCommand(float xPosition, float yPosition, float Width, float Height, Colour RectColour);
        DrawRectangleCommand(float xPosition, float yPosition, float Width, float Height, Colour RectColour, bool Normalized);

        virtual void Execute(RendererBase* Renderer) override;
    private:
            Vec2 mPosition;
            float mWidth = 1.f;
            float mHeight = 1.f;
            Colour mColour;
            bool mNormalized = false;
    };

}