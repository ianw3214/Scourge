#pragma once

#include <vector>
#include <memory>

#include "shade/instance/service/service.h"

namespace Shade {

    struct InputEvent;
    class Module;
    class RenderCommand;

    // ======================================
    class State {
    public:
        State();
        ~State();

        void UpdateModules(float deltaSeconds);
        void RenderModules(std::vector<std::unique_ptr<RenderCommand>>& commandQueue);
        void HandleEvent(const InputEvent& event);

        void AddModule(std::unique_ptr<Module> module);
    private:
        std::vector<std::unique_ptr<Module>> mModules;
    };

    // ======================================
    class StateChangeService : public Service {
    public:
        StateChangeService();
        ~StateChangeService();

        void SetNextState(std::unique_ptr<State> state);
        std::unique_ptr<State> GetNextState();
    private:
        std::unique_ptr<State> mNextState;
    };

        
}