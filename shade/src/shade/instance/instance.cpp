#include "instance.h"

#include <chrono>
#include <vector>

#include "shade/window/window.h"
#include "shade/input/input.h"
#include "shade/graphics/command/command.h"
#include "shade/graphics/renderer.h"
#include "shade/module/state.h"

// ======================================
Shade::GameInstance::GameInstance(std::unique_ptr<State> InitialState) 
{
    mMainWindow = std::make_unique<Shade::Window>();
    mInputHandler = std::make_unique<Shade::InputHandler>(*this);
    mRenderer = std::make_unique<Shade::RendererBase>();
    mCurrentState = std::move(InitialState);

    mRenderer->InitializeDefaultShaders();
}

// ======================================
Shade::GameInstance::~GameInstance() = default;

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
        mCurrentState->UpdateModules(mDeltaSeconds);

        mRenderer->Clear();
        std::vector<std::unique_ptr<RenderCommand>> RenderCommands;
        mCurrentState->RenderModules(RenderCommands);
        // TODO: Once this is multithreaded, should also check previous render queue is done
        mRenderer->SwapCommandQueue(RenderCommands);
        mRenderer->ProcessCommandQueue();

        mMainWindow->Update();
        mDeltaSeconds = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - updateStart).count()) / 1000000.f;
    }
}

// ======================================
void Shade::GameInstance::Notify(GameNotification Notification)
{
    if (Notification == GameNotification::Quit)
    {
        mRunning = false;
    }
}

// ======================================
void Shade::GameInstance::NotifyRenderer(std::unique_ptr<RenderCommand> Command)
{
    mRenderer->QueueCommand(std::move(Command));
}