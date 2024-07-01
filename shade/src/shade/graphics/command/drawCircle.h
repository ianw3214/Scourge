#pragma once

#include "shade/graphics/command/command.h"

#include "shade/common/vec.h"
#include "shade/common/colour.h"

namespace Shade {

    class DrawCircleCommand : public RenderCommand {
    public:
        DrawCircleCommand();
        DrawCircleCommand(Vec2 point);
        DrawCircleCommand(Vec2 point, float radius);
        DrawCircleCommand(Vec2 point, float radius, Colour colour);
        DrawCircleCommand(Vec2 point, float radius, Colour colour, bool normalized);
        DrawCircleCommand(float x, float y);
        DrawCircleCommand(float x, float y, float radius);
        DrawCircleCommand(float x, float y, float radius, Colour colour);
        DrawCircleCommand(float x, float y, float radius, Colour colour, bool normalized);

        virtual void Execute(RendererBase* renderer, const CameraInfo& camera) override;
    private:
        Vec2 mPoint;
        float mRadius;
        Colour mColour;
        bool mNormalized = false;
    };

}