#pragma once

namespace Shade {

    class RendererBase;

    class RenderCommand {
    public:
        RenderCommand();
        virtual ~RenderCommand();
        virtual void Execute(RendererBase* renderer) = 0;
    };

}