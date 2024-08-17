#pragma once

#include "shade/editor/editorBase.h"

namespace Shade {

    class Entity;

    // ======================================
    class EntityEditor : public Shade::EditorBase {
    public:
        EntityEditor();

        virtual void OnEnter() override;
        virtual void OnExit() override;

        virtual void Update(float deltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue) override;
        virtual bool HandleEvent(const Shade::InputEvent& event) override;

        void SetEntityData(std::unique_ptr<Entity> entityData);
        std::unique_ptr<Entity>& GetEntityData();
        void CreateEmptyEntity();
        void OpenFile();
        void SaveFile();

    private:
        // Bypass resource system for storage since we want the editor to own this entity data
        std::unique_ptr<Entity> mEntityData = nullptr;

        // Input states
        bool mControlPressed = false;
    };

}