#include "state.h"

#include "shade/module/module.h"
#include "shade/graphics/command/command.h"

// ======================================
Shade::State::State()
{

}

// ======================================
Shade::State::~State() = default;

// ======================================
void Shade::State::InitializeModules()
{
    for (const auto& Module : mModules)
    {
        Module->Initialize();
    }
}

// ======================================
void Shade::State::UpdateModules(float deltaSeconds)
{
    for (const auto& Module : mModules)
    {
        Module->Update(deltaSeconds);
    }
}

// ======================================
void Shade::State::RenderModules(std::vector<std::unique_ptr<RenderCommand>>& commandQueue)
{
    for (const auto& Module : mModules)
    {
        Module->Render(commandQueue);
    }
}

// ======================================
void Shade::State::HandleEvent(const InputEvent& event)
{
    for (const auto& module : mModules)
    {
        const bool shouldContinue = module->HandleEvent(event);
        if (!shouldContinue)
        {
            break;
        }
    }
}

// ======================================
void Shade::State::AddModule(std::unique_ptr<Module> module)
{
    mModules.emplace_back(std::move(module));
}

// ======================================
Shade::StateChangeService::StateChangeService()
    : Shade::Service("State change service")
{

}

// ======================================
Shade::StateChangeService::~StateChangeService()
{

}

// ======================================
void Shade::StateChangeService::SetNextState(std::unique_ptr<State> state)
{
    mNextState = std::move(state);
}

// ======================================
std::unique_ptr<Shade::State> Shade::StateChangeService::GetNextState()
{
    if (mNextState != nullptr)
    {
        return std::move(mNextState);
    }
    return nullptr;
}