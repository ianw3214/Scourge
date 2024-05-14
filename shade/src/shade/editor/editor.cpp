#include "editor.h"

#include "shade/editor/editorBase.h"

// ======================================
Shade::EditorModule::EditorModule() = default;
// ======================================
Shade::EditorModule::~EditorModule() = default;

// ======================================
void Shade::EditorModule::Update(float deltaSeconds) 
{

}

// ======================================
void Shade::EditorModule::Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) 
{

}

// ======================================
bool Shade::EditorModule::HandleEvent(const InputEvent& event) 
{
    return false;
}

// ======================================
void Shade::EditorModule::RegisterEditor(std::unique_ptr<EditorBase> editor)
{
    // TODO: Checking that no duplicate editors are registered
    mEditors.emplace_back(std::move(editor));
}