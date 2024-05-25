#include "editor.h"

#include <cassert>

#include "shade/editor/editorBase.h"
#include "shade/editor/ui/overview.h"
#include "shade/input/event.h"
#include "shade/instance/service/provider.h"
#include "shade/graphics/imgui/service.h"
#include "shade/logging/logService.h"

// ======================================
Shade::EditorService::EditorService()
    : Service("Editor Service")
{

}

// ======================================
void Shade::EditorService::RegisterEditor(std::unique_ptr<EditorBase> editor)
{
    if (mEditors.empty())
    {
        editor->OnEnter();
    }

    // TODO: Checking that no duplicate editors are registered
    mEditors.emplace_back(std::move(editor));
}

// ======================================
const std::vector<std::unique_ptr<Shade::EditorBase>>& Shade::EditorService::GetEditors() const
{
    return mEditors;
}

// ======================================
const std::unique_ptr<Shade::EditorBase>& Shade::EditorService::GetCurrentEditor() const
{
    assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
    return mEditors[mCurrentEditor];
}

// ======================================
std::unique_ptr<Shade::EditorBase>& Shade::EditorService::GetCurrentEditorMutable()
{
    assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
    return mEditors[mCurrentEditor];
}

// ======================================
size_t Shade::EditorService::GetCurrentEditorIndex() const
{
    assert(mCurrentEditor < mEditors.size() && "Current editor index out of bounds");
    return mCurrentEditor;
}

// ======================================
void Shade::EditorService::SetRunGameCallback(std::function<void()> runGameCallback)
{
    mRunGameCallback = runGameCallback;
}

// ======================================
void Shade::EditorService::SetStopGameCallback(std::function<void()> stopGameCallback)
{
    mStopGameCallback = stopGameCallback;
}

// ======================================
void Shade::EditorService::RunGame()
{
    mRunGameCallback();
}

// ======================================
//  - This needs to be done as a callback because the service doesn't have access to the instance
//      - therefore it is unable to create a new state on it's own
//  - Technically, we should be able to always just create an editor state here and it should work properly
void Shade::EditorService::StopGame()
{
    mStopGameCallback();
}

// ======================================
Shade::EditorModule::EditorModule()
{
    ImGuiService* imguiService = ServiceProvider::GetCurrentProvider()->GetService<ImGuiService>();
    imguiService->RegisterWindow(std::make_unique<EditorOverviewWindow>(*this));
}

// ======================================
Shade::EditorModule::~EditorModule() = default;

// ======================================
void Shade::EditorModule::Update(float deltaSeconds) 
{
    Shade::EditorService* editorService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EditorService>();
    return editorService->GetCurrentEditorMutable()->Update(deltaSeconds);
}

// ======================================
void Shade::EditorModule::Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) 
{
    Shade::EditorService* editorService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EditorService>();
    return editorService->GetCurrentEditorMutable()->Render(commandQueue);
}

// ======================================
bool Shade::EditorModule::HandleEvent(const InputEvent& event) 
{
    Shade::EditorService* editorService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EditorService>();
    return editorService->GetCurrentEditorMutable()->HandleEvent(event);
    return false;
}

// ======================================
Shade::EditorContextModule::EditorContextModule() = default;

// ======================================
Shade::EditorContextModule::~EditorContextModule() = default;

// ======================================
bool Shade::EditorContextModule::HandleEvent(const InputEvent& event) 
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    if (event.mType == Shade::InputEventType::KEY)
    {
        if (event.mKeyEvent == Shade::KeyEventType::PRESS && event.mKeyCode == Shade::KeyCode::SHADE_KEY_F5)
        {
            Shade::EditorService* editorService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EditorService>();
            editorService->StopGame();
            return true;
        }
    }
    return false;
}