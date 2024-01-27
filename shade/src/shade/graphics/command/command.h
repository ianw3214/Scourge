#pragma once

namespace Shade {

    class RendererBase;
    class CameraInfo;

    class RenderCommand {
    public:
        RenderCommand();
        virtual ~RenderCommand();
        virtual void Execute(RendererBase* renderer, const CameraInfo& camera) = 0;
    };

}