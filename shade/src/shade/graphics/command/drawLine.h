#pragma once

#include "shade/graphics/command/command.h"

#include "shade/common/vec.h"
#include "shade/common/colour.h"

namespace Shade {

    class DrawLineCommand : public RenderCommand {
    public:
        DrawLineCommand();
        DrawLineCommand(Vec2 Point1, Vec2 Point2);
        DrawLineCommand(Vec2 Point1, Vec2 Point2, Colour LineColour);
        DrawLineCommand(Vec2 Point1, Vec2 Point2, Colour LineColour, bool Normalized);
        DrawLineCommand(float Point1x, float Point1y, float Point2x, float Point2y);
        DrawLineCommand(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour);
        DrawLineCommand(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour, bool Normalized);

        virtual void Execute(RendererBase* Renderer) override;
    private:
        Vec2 mPoint1;
        Vec2 mPoint2;
        Colour mColour;
        bool mNormalized = false;
    };

}