#include "instance.h"

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
    // TODO: Replace with actual delta time
    constexpr float deltaSeconds = 1.0f / 24.0f;

    mRunning = true;
    while (mRunning)
    {
        mInputHandler->Update();
        while (std::optional<InputEvent> NextEvent = mInputHandler->GetNextEvent())
        {
            mCurrentState->HandleEvent(*NextEvent);
        }
        mCurrentState->UpdateModules(deltaSeconds);

        mRenderer->Clear();

        std::vector<std::unique_ptr<RenderCommand>> RenderCommands;
        mCurrentState->RenderModules(RenderCommands);

        // TODO: Once this is multithreaded, should also check previous render queue is done
        mRenderer->SwapCommandQueue(RenderCommands);
        mRenderer->ProcessCommandQueue();

        mMainWindow->Update();
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