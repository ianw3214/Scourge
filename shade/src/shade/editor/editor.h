#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "shade/instance/service/service.h"
#include "shade/module/module.h"
#include "shade/module/state.h"

namespace Shade {

    class EditorBase;
    class ServiceProvider;

    // ======================================
    //  TODO: Maybe the configuration can be set through an "editor service" instead
    //  - By using a service, editor state/configuration can be set even when the instance goes into a game
    struct EditorConfiguration {
        std::vector<std::unique_ptr<EditorBase>> mEditors;
    };

    // ======================================
    class EditorService : public Shade::Service {
    public:
        EditorService();

        void RegisterEditor(std::unique_ptr<EditorBase> editor);
        const std::vector<std::unique_ptr<EditorBase>>& GetEditors() const;
        const std::unique_ptr<EditorBase>& GetCurrentEditor() const;
        std::unique_ptr<EditorBase>& GetCurrentEditorMutable();
        size_t GetCurrentEditorIndex() const;

        void SetRunGameCallback(std::function<void()> runGameCallback);
        void RunGame();
    private:
        std::vector<std::unique_ptr<EditorBase>> mEditors;
        size_t mCurrentEditor = 0;
        
        std::function<void()> mRunGameCallback;
    };

    // ======================================
    class EditorModule : public Shade::Module {
    public:
        EditorModule(EditorConfiguration& config);
        ~EditorModule();

        virtual void Update(float deltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) override;
        virtual bool HandleEvent(const InputEvent& event) override;
    };

    // ======================================
    class EditorState : public Shade::State {
    public:
        EditorState(Shade::ServiceProvider& serviceProviderRef, EditorConfiguration& config) : Shade::State() {
            AddModule(std::make_unique<EditorModule>(config));
        }
    };

}