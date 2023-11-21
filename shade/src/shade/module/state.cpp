#include "state.h"

#include "shade/module/module.h"
#include "shade/graphics/command/command.h"

// ======================================
Shade::State::State() = default;

// ======================================
Shade::State::~State() = default;

// ======================================
void Shade::State::UpdateModules(float DeltaSeconds)
{
    for (const auto& Module : mModules)
    {
        Module->Update(DeltaSeconds);
    }
}

// ======================================
void Shade::State::RenderModules(std::vector<std::unique_ptr<RenderCommand>>& CommandQueue)
{
    for (const auto& Module : mModules)
    {
        Module->Render(CommandQueue);
    }
}

// ======================================
void Shade::State::HandleEvent(const InputEvent& Event)
{
    for (const auto& Module : mModules)
    {
        const bool ShouldContinue = Module->HandleEvent(Event);
        if (!ShouldContinue)
        {
            break;
        }
    }
}

// ======================================
void Shade::State::AddModule(std::unique_ptr<Module> NewModule)
{
    mModules.emplace_back(std::move(NewModule));
}