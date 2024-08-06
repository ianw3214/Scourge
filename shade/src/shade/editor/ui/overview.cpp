#include "overview.h"

#include <imgui.h>

#include "shade/editor/editor.h"
#include "shade/editor/editorBase.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

namespace {
    // TODO: This is pretty unsafe and hacky, perhaps look for a better solution here
    bool VectorOfStringGetter(void* data, int idx, const char** out_text)
    {
        std::unique_ptr<Shade::EditorBase>* editors = (std::unique_ptr<Shade::EditorBase>*)data;
        std::unique_ptr<Shade::EditorBase>& currEditor = editors[idx];

        *out_text = currEditor->GetName().c_str();
        return true;
    }
}

// ======================================
Shade::EditorOverviewWindow::EditorOverviewWindow()
    : ImGuiWindow("Editor Overview Window")
{

}

// ======================================
void Shade::EditorOverviewWindow::Draw()
{
    Shade::EditorService* editorService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EditorService>();
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    
    ImGui::Begin("Editor");
    const std::vector<std::unique_ptr<EditorBase>>& editors = editorService->GetEditors();
    if (editors.size() == 0)
    {
        ImGui::Text("No editors found...");
    }
    else
    {
        int originalListboxItem = static_cast<int>(editorService->GetCurrentEditorIndex());
        int currListboxItem = originalListboxItem;
        ImGui::ListBox("Editors", &currListboxItem, VectorOfStringGetter, (void*)editors.data(), editors.size());
        // If selected item changes, handle changing the current editor
        if (originalListboxItem != currListboxItem)
        {
            editorService->ChangeEditor(static_cast<size_t>(currListboxItem));
            logger->LogInfo(std::string("Switched to editor: ") + editorService->GetCurrentEditor()->GetName());
        }
    }

    if (ImGui::Button("Run Game"))
    {
        editorService->RunGame();
    }

    ImGui::End();
}
