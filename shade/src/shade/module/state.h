#pragma once

#include <vector>
#include <memory>

namespace Shade {

    class Module;

    class State {
    public:
        State();
        ~State();

        void UpdateModules(float DeltaSeconds);
        void RenderModules();
    protected:
        void AddModule(Module&& NewModule);
    private:
        std::vector<std::unique_ptr<Module>> mModules;
    };
        
}