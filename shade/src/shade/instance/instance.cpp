#include "instance.h"

#include "shade/window/window.h"
#include "shade/input/input.h"
#include "shade/graphics/renderer.h"
#include "shade/module/state.h"

// ======================================
Shade::GameInstance::GameInstance(std::unique_ptr<State> InitialState) 
{
    mMainWindow = std::make_unique<Shade::Window>();
    mInputHandler = std::make_unique<Shade::InputHandler>(*this);
    mRenderer = std::make_unique<Shade::Renderer>();
    mCurrentState = std::move(InitialState);
}

// ======================================
Shade::GameInstance::~GameInstance()
{

}

// ======================================
void Shade::GameInstance::Run()
{
    mRunning = true;
    while (mRunning)
    {
        mInputHandler->Update();
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