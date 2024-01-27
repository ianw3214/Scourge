#pragma once

#include "shade/graphics/command/command.h"

#include "shade/common/vec.h"
#include "shade/common/colour.h"

namespace Shade {

    class DrawLineCommand : public RenderCommand {
    public:
        DrawLineCommand();
        DrawLineCommand(Vec2 point1, Vec2 point2);
        DrawLineCommand(Vec2 point1, Vec2 point2, Colour colour);
        DrawLineCommand(Vec2 point1, Vec2 point2, Colour colour, bool normalized);
        DrawLineCommand(float point1x, float point1y, float point2x, float point2y);
        DrawLineCommand(float point1x, float point1y, float point2x, float point2y, Colour colour);
        DrawLineCommand(float point1x, float point1y, float point2x, float point2y, Colour colour, bool normalized);

        virtual void Execute(RendererBase* renderer, const CameraInfo& camera) override;
    private:
        Vec2 mPoint1;
        Vec2 mPoint2;
        Colour mColour;
        bool mNormalized = false;
    };

}