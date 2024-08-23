#include "editor.h"

#include <cassert>

#include "shade/editor/editorBase.h"
#include "shade/editor/entity/entityEditor.h"
#include "shade/editor/ui/overview.h"
#include "shade/input/event.h"
#include "shade/instance/service/provider.h"
#include "shade/graphics/imgui/service.h"
#include "shade/logging/logService.h"

// ======================================
// - This needs to be created after the ImGui service so that the window can be properly initialized
Shade::EditorService::EditorService()
    : Service("Editor Service")
{
    ImGuiService* imguiService = ServiceProvider::GetCurrentProvider()->GetService<ImGuiService>();
    imguiService->RegisterWindow(std::make_unique<EditorOverviewWindow>());
}

// ======================================
void Shade::EditorService::RegisterDefaultEditors()
{
    RegisterEditor(std::make_unique<EntityEditor>());
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
bool Shade::EditorService::IsRunningGame() const
{
    return mRunningGame;
}

// ======================================
void Shade::EditorService::ChangeEditor(size_t newEditorIndex)
{
    // There should always be a valid current editor, so no need to check validty of current editor
    if (mCurrentEditor == newEditorIndex)
    {
        // TODO: Throw a warning
    }
    if (newEditorIndex >= mEditors.size())
    {
        // TODO: Throw a warning
    }
    GetCurrentEditor()->OnExit();
    mCurrentEditor = newEditorIndex;
    GetCurrentEditor()->OnEnter();
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
    GetCurrentEditor()->OnExit();

    mRunGameCallback();
    mRunningGame = true;
}

// ======================================
//  - This needs to be done as a callback because the service doesn't have access to the instance
//      - therefore it is unable to create a new state on it's own
//  - Technically, we should be able to always just create an editor state here and it should work properly
void Shade::EditorService::StopGame()
{
    mStopGameCallback();
    mRunningGame = false;

    GetCurrentEditor()->OnEnter();
}

// ======================================
Shade::EditorModule::EditorModule()
{
    
}

// ======================================
Shade::EditorModule::~EditorModule() = default;

// ======================================
void Shade::EditorModule::Initialize()
{
    Shade::EditorService* editorService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EditorService>();
    return editorService->GetCurrentEditorMutable()->OnEnter();
}

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