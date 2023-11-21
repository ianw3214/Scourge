#pragma once

#include <vector>
#include <memory>

namespace Shade {

    struct InputEvent;
    class RenderCommand;

    class Module {
    public:
        Module();
        virtual ~Module();

        virtual void Update(float DeltaSeconds);
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& CommandQueue);
        virtual bool HandleEvent(const InputEvent& Event);
    };

}