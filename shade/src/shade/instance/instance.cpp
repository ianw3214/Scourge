#include "instance.h"

#include <chrono>
#include <vector>

#include "shade/editor/editor.h"
#include "shade/file/fileSystem.h"
#include "shade/game/entity/factory.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/command.h"
#include "shade/graphics/imgui/service.h"
#include "shade/graphics/imgui/wrapper.h"
#include "shade/graphics/renderer.h"
#include "shade/input/input.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/logging/logWindow.h"
#include "shade/module/state.h"
#include "shade/resource/manager.h"
#include "shade/window/window.h"

// ======================================
Shade::GameInstance::GameInstance() 
{
    ServiceProvider::RegisterProvider(this);
    RegisterService(new FileSystem());
    RegisterService(new ResourceManager());
    RegisterService(new CameraService());
    RegisterService(new LogService());
    RegisterService(new ImGuiService());
    RegisterService(new StateChangeService());

    // Temporarily register services
    //  - There's GOT TO be a better way to do this... QQ
    // TODO: This has to take debug builds into account as well
    ImGuiService* imguiService = GetService<ImGuiService>();
    imguiService->RegisterWindow(std::make_unique<LogWindow>());

    mMainWindow = std::make_unique<Shade::Window>();
    mInputHandler = std::make_unique<Shade::InputHandler>(*this);
    mRenderer = std::make_unique<Shade::RendererBase>();

    // TODO: Perhaps all services should have an "initialization" pass that can be done here 
    mRenderer->InitializeDefaultShaders();

    // Register gameplay related services
    RegisterService(new EntityFactory());

#ifdef BUILD_SHADE_EDITOR
    RegisterService(new EditorService());
#endif

    LogService* logService = GetService<LogService>();
    logService->LogInfo("Shade engine initialized!");
}

// ======================================
Shade::GameInstance::~GameInstance() = default;

// ======================================
//  - Returns a reference to the newly set state
//      - The returned reference can be used to modify the state further, but should not be stored long-term
std::unique_ptr<Shade::State>& Shade::GameInstance::SetState(std::unique_ptr<State> state)
{
    if (mCurrentState != nullptr)
    {
        // TODO: Might want to handle this somehow
    }
    mCurrentState = std::move(state);
    return mCurrentState;
}

// ======================================
void Shade::GameInstance::Run()
{
    mRunning = true;
    while (mRunning)
    {
        auto updateStart = std::chrono::system_clock::now();

        mInputHandler->Update();
        while (std::optional<InputEvent> NextEvent = mInputHandler->GetNextEvent())
        {
            mCurrentState->HandleEvent(*NextEvent);
        }
        mCurrentState->UpdateModules(mdeltaSeconds);

        mRenderer->Clear();
        ImGuiWrapper::StartFrame();

        std::vector<std::unique_ptr<RenderCommand>> RenderCommands;
        mCurrentState->RenderModules(RenderCommands);
        // TODO: Once this is multithreaded, should also check previous render queue is done
        mRenderer->SwapCommandQueue(RenderCommands);
        mRenderer->ProcessCommandQueue();

        mMainWindow->Update();
        mdeltaSeconds = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - updateStart).count()) / 1000000.f;

        // TODO: Proper reset of instance state (delta time etc)
        //  - Also proper enter/exit of states to handle initialization/cleanup
        Shade::StateChangeService* stateChanger = GetService<Shade::StateChangeService>();
        std::unique_ptr<State> nextState = stateChanger->GetNextState();
        if (nextState != nullptr)
        {
            mCurrentState = std::move(nextState);
        }
    }
}

// ======================================
void Shade::GameInstance::Notify(GameNotification notification)
{
    if (notification == GameNotification::Quit)
    {
        mRunning = false;
    }
}

// ======================================
void Shade::GameInstance::NotifyRenderer(std::unique_ptr<RenderCommand> command)
{
    mRenderer->QueueCommand(std::move(command));
}