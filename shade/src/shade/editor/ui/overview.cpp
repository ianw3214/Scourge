#include "overview.h"

#include <imgui.h>

#include "shade/editor/editor.h"
#include "shade/editor/editorBase.h"

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
Shade::EditorOverviewWindow::EditorOverviewWindow(EditorModule& editor)
    : mEditorRef(editor)
{

}

// ======================================
void Shade::EditorOverviewWindow::Draw()
{
    ImGui::Begin("Editor");

    const std::vector<std::unique_ptr<EditorBase>>& editors = mEditorRef.GetEditors();
    if (editors.size() == 0)
    {
        ImGui::Text("No editors found...");
    }
    else
    {
        int currListboxItem = static_cast<int>(mEditorRef.GetCurrentEditorIndex());
        ImGui::ListBox("Editors", &currListboxItem, VectorOfStringGetter, (void*)editors.data(), editors.size());
        // TODO: If selected item changes, need to handle editor enter/exit
    }
    ImGui::End();
}
