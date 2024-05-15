#include "editor.h"

#include "shade/editor/editorBase.h"
#include "shade/editor/ui/overview.h"
#include "shade/instance/service/provider.h"
#include "shade/graphics/imgui/service.h"

// ======================================
Shade::EditorModule::EditorModule(EditorConfiguration& config)
{
    ImGuiService* imguiService = ServiceProvider::GetCurrentProvider()->GetService<ImGuiService>();
    imguiService->RegisterWindow(std::make_unique<EditorOverviewWindow>(*this));

    for (std::unique_ptr<EditorBase>& editor : config.mEditors)
    {
        RegisterEditor(std::move(editor));
    }
}

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

// ======================================
const std::vector<std::unique_ptr<Shade::EditorBase>>& Shade::EditorModule::GetEditors() const
{
    return mEditors;
}

// ======================================
const std::unique_ptr<Shade::EditorBase>& Shade::EditorModule::GetCurrentEditor() const
{
    // TODO: Bounds checking
    return mEditors[mCurrentEditor];
}

// ======================================
size_t Shade::EditorModule::GetCurrentEditorIndex() const
{
    // TODO: Asserts on index bounds
    return mCurrentEditor;
}