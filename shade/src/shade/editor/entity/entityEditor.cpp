#include "entityEditor.h"

#include <imgui/imgui.h>

#include "shade/file/fileSystem.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/imgui/service.h"
#include "shade/graphics/imgui/window.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
class EntityEditorWindow : public Shade::ImGuiWindow {
public:
    EntityEditorWindow(Shade::EntityEditor& entityEditorRef) : ImGuiWindow("Entity editor window"), mEntityEditorRef(entityEditorRef) {}
    void Draw() override {
        Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        Shade::FileSystem* fileSystem = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FileSystem>();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        ImGui::Begin("Entity Editor", nullptr, window_flags);

        if (ImGui::BeginMenuBar()) {
            // TODO: Look into "nativefiledialog" to actually hook up to file dialogs
            //  - https://github.com/mlabbe/nativefiledialog
            //  - For now, just always open/save to the same file and manually rename to work with different map files
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    // TODO: Implement
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    // TODO: Implement
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    // TODO: Implement
                }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {
                    // TODO: Implement
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();

            std::unique_ptr<Shade::Entity>& entityData = mEntityEditorRef.GetEntityData();
            if (entityData != nullptr)
            {
                
            }
        }

        ImGui::End();
    }
private:
    Shade::EntityEditor& mEntityEditorRef;
};

// ======================================
Shade::EntityEditor::EntityEditor()
    : Shade::EditorBase("Entity Editor")
{

}

// ======================================
void Shade::EntityEditor::OnEnter()
{
    Shade::ImGuiService* imguiService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ImGuiService>();
    imguiService->RegisterWindow(std::make_unique<EntityEditorWindow>(*this));
}

// ======================================
void Shade::EntityEditor::OnExit()
{
    // TODO: The string used here might want to use a shared const to avoid deleting the wrong window
    Shade::ImGuiService* imguiService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ImGuiService>();
    imguiService->DeleteWindow("Entity editor window");
}

// ======================================
void Shade::EntityEditor::Update(float deltaSeconds)
{

}

// ======================================
void Shade::EntityEditor::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{

}

// ======================================
bool Shade::EntityEditor::HandleEvent(const Shade::InputEvent& event)
{
    return false;
}

// ======================================
void Shade::EntityEditor::SetEntityData(std::unique_ptr<Entity> entityData)
{
    mEntityData = std::move(entityData);
}

// ======================================
std::unique_ptr<Shade::Entity>& Shade::EntityEditor::GetEntityData()
{
    return mEntityData;
}
