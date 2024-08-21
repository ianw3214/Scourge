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

        // There can be a delay between creation vs when the module is valid to use
        //  - Initialize will be called when the module is ready to actually be used
        //  - Generally, this means that the previous state was cleaned up
        virtual void Initialize();

        virtual void Update(float deltaSeconds);
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue);
        virtual bool HandleEvent(const InputEvent& event);
    };

}