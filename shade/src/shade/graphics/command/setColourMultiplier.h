#pragma once

#include <string>

#include "shade/graphics/command/command.h"
#include "shade/common/colour.h"

namespace Shade {

    class SetColourMultiplierCommand : public RenderCommand {
    public:
        SetColourMultiplierCommand(Colour colour);

        virtual void Execute(RendererBase* renderer, const CameraInfo& camera) override;
    private:
        Colour mColour = { 1.f, 1.f, 1.f };
    };

    class ResetColourMultiplierCommand : public RenderCommand {
    public:
        ResetColourMultiplierCommand();

        virtual void Execute(RendererBase* renderer, const CameraInfo& camera) override;
    };

}