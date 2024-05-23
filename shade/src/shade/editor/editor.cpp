#include "editor.h"

#include <cassert>

#include "shade/editor/editorBase.h"
#include "shade/editor/ui/overview.h"
#include "shade/instance/service/provider.h"
#include "shade/graphics/imgui/service.h"

// ======================================
Shade::EditorService::EditorService()
    : Service("Editor Service")
{

}

// ======================================
void Shade::EditorService::SetRunGameCallback(std::function<void()> runGameCallback)
{
    mRunGameCallback = runGameCallback;
}

// ======================================
void Shade::EditorService::RunGame()
{
    mRunGameCallback();
}

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
    if (!mEditors.empty())
    {
        assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
        return mEditors[mCurrentEditor]->Update(deltaSeconds);
    }
}

// ======================================
void Shade::EditorModule::Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) 
{
    if (!mEditors.empty())
    {
        assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
        return mEditors[mCurrentEditor]->Render(commandQueue);
    }
}

// ======================================
bool Shade::EditorModule::HandleEvent(const InputEvent& event) 
{
    if (!mEditors.empty())
    {
        assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
        return mEditors[mCurrentEditor]->HandleEvent(event);
    }
    return false;
}

// ======================================
void Shade::EditorModule::RegisterEditor(std::unique_ptr<EditorBase> editor)
{
    if (mEditors.empty())
    {
        editor->OnEnter();
    }

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
    assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
    return mEditors[mCurrentEditor];
}

// ======================================
size_t Shade::EditorModule::GetCurrentEditorIndex() const
{
    assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
    return mCurrentEditor;
}