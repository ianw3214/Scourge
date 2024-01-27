#include "instance.h"

#include <chrono>
#include <vector>

#include "shade/window/window.h"
#include "shade/input/input.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/command.h"
#include "shade/graphics/renderer.h"
#include "shade/logging/logService.h"
#include "shade/module/state.h"

// ======================================
Shade::GameInstance::GameInstance() 
{
    mMainWindow = std::make_unique<Shade::Window>();
    mInputHandler = std::make_unique<Shade::InputHandler>(*this);
    mRenderer = std::make_unique<Shade::RendererBase>();

    ServiceProvider::RegisterProvider(this);
    RegisterService(new ResourceManager());
    RegisterService(new CameraService());
    RegisterService(new LogService());

    mRenderer->InitializeDefaultShaders();
}

// ======================================
Shade::GameInstance::~GameInstance() = default;

// ======================================
void Shade::GameInstance::SetState(std::unique_ptr<State> state)
{
    if (mCurrentState != nullptr)
    {
        // TODO: Might want to handle this somehow
    }
    mCurrentState = std::move(state);
}

// ======================================
void Shade::GameInstance::Run()
{
    // TODO: Temp code - remove soon
    mRenderer->SetClearColor(1.0, 1.0, 1.0, 1.0);

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
        std::vector<std::unique_ptr<RenderCommand>> RenderCommands;
        mCurrentState->RenderModules(RenderCommands);
        // TODO: Once this is multithreaded, should also check previous render queue is done
        mRenderer->SwapCommandQueue(RenderCommands);
        mRenderer->ProcessCommandQueue();

        mMainWindow->Update();
        mdeltaSeconds = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - updateStart).count()) / 1000000.f;
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