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

        void UpdateModules(float deltaSeconds);
        void RenderModules(std::vector<std::unique_ptr<RenderCommand>>& commandQueue);
        void HandleEvent(const InputEvent& event);
    protected:
        void AddModule(std::unique_ptr<Module> module);
    private:
        std::vector<std::unique_ptr<Module>> mModules;
    };
        
}