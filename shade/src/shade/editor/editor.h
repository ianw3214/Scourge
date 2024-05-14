#pragma once

#include <limits>
#include <vector>
#include <memory>

#include "shade/module/module.h"
#include "shade/module/state.h"

namespace Shade {

    class EditorBase;
    class ServiceProvider;

    class EditorModule : public Shade::Module {
    public:
        EditorModule();
        ~EditorModule();

        virtual void Update(float deltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) override;
        virtual bool HandleEvent(const InputEvent& event) override;

        void RegisterEditor(std::unique_ptr<EditorBase> editor);
    private:
        std::vector<std::unique_ptr<EditorBase>> mEditors;
        size_t mCurrentEditor = std::numeric_limits<size_t>::max();
    };

    class EditorState : public Shade::State {
    public:
        EditorState(Shade::ServiceProvider& serviceProviderRef) : Shade::State() {
            AddModule(std::make_unique<EditorModule>());
        }
    };

}