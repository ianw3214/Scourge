#include "state.h"

#include "shade/module/module.h"

// ======================================
Shade::State::State()
{

}

// ======================================
Shade::State::~State()
{

}

// ======================================
void Shade::State::UpdateModules(float DeltaSeconds)
{
    for (const auto& Module : mModules)
    {
        Module->Update(DeltaSeconds);
    }
}

void Shade::State::RenderModules()
{
    for (const auto& Module : mModules)
    {
        Module->Render();
    }
}

// ======================================
void Shade::State::AddModule(Module&& NewModule)
{
    mModules.emplace_back(std::make_unique<Module>(NewModule));
}