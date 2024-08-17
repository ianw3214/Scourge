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
    class EditorService : public Shade::Service {
    public:
        EditorService();
        void RegisterDefaultEditors();

        void RegisterEditor(std::unique_ptr<EditorBase> editor);
        const std::vector<std::unique_ptr<EditorBase>>& GetEditors() const;
        const std::unique_ptr<EditorBase>& GetCurrentEditor() const;
        std::unique_ptr<EditorBase>& GetCurrentEditorMutable();
        size_t GetCurrentEditorIndex() const;

        void ChangeEditor(size_t newEditorIndex);

        void SetRunGameCallback(std::function<void()> runGameCallback);
        void SetStopGameCallback(std::function<void()> stopGameCallback);
        void RunGame();
        void StopGame();
    private:
        std::vector<std::unique_ptr<EditorBase>> mEditors;
        size_t mCurrentEditor = 0;
        
        std::function<void()> mRunGameCallback;
        std::function<void()> mStopGameCallback;
    };

    // ======================================
    class EditorModule : public Shade::Module {
    public:
        EditorModule();
        ~EditorModule();

        virtual void Update(float deltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) override;
        virtual bool HandleEvent(const InputEvent& event) override;
    };

    // ======================================
    //  - This module should be put into games being run via the editor
    //  - Retains any state necessary for the editor and allows the game to return to the editor
    class EditorContextModule : public Shade::Module {
    public:
        EditorContextModule();
        ~EditorContextModule();

        virtual bool HandleEvent(const InputEvent& event) override;
    };

    // ======================================
    class EditorState : public Shade::State {
    public:
        EditorState(Shade::ServiceProvider& serviceProviderRef) : Shade::State() {
            AddModule(std::make_unique<EditorModule>());
        }
    };

}