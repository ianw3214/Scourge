#pragma once

#include <vector>
#include <memory>

namespace Shade {

    struct InputEvent;
    class Module;
    class RenderCommand;

    class State {
    public:
        State();
        ~State();

        void UpdateModules(float DeltaSeconds);
        void RenderModules(std::vector<std::unique_ptr<RenderCommand>>& CommandQueue);
        void HandleEvent(const InputEvent& Event);
    protected:
        void AddModule(std::unique_ptr<Module> NewModule);
    private:
        std::vector<std::unique_ptr<Module>> mModules;
    };
        
}