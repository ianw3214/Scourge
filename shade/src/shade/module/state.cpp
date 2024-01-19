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