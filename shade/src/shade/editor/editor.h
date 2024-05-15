#pragma once

#include <vector>
#include <memory>

#include "shade/module/module.h"
#include "shade/module/state.h"

namespace Shade {

    class EditorBase;
    class ServiceProvider;

    // ======================================
    //  TODO: Maybe the configuration can be set through an "editor service" instead
    struct EditorConfiguration {
        std::vector<std::unique_ptr<EditorBase>> mEditors;
    };

    // ======================================
    class EditorModule : public Shade::Module {
    public:
        EditorModule(EditorConfiguration& config);
        ~EditorModule();

        virtual void Update(float deltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) override;
        virtual bool HandleEvent(const InputEvent& event) override;

        void RegisterEditor(std::unique_ptr<EditorBase> editor);

        const std::vector<std::unique_ptr<EditorBase>>& GetEditors() const;
        const std::unique_ptr<EditorBase>& GetCurrentEditor() const;
        size_t GetCurrentEditorIndex() const;
    private:
        std::vector<std::unique_ptr<EditorBase>> mEditors;
        size_t mCurrentEditor = 0;
    };

    // ======================================
    class EditorState : public Shade::State {
    public:
        EditorState(Shade::ServiceProvider& serviceProviderRef, EditorConfiguration& config) : Shade::State() {
            AddModule(std::make_unique<EditorModule>(config));
        }
    };

}