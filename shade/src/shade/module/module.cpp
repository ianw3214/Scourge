#include "module.h"

#include "shade/graphics/command/command.h"

// ======================================
Shade::Module::Module() = default;

// ======================================
Shade::Module::~Module() = default;

// ======================================
void Shade::Module::Update(float deltaSeconds)
{

}

// ======================================
void Shade::Module::Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue)
{

}

// ======================================
bool Shade::Module::HandleEvent(const InputEvent& event)
{
    // Could consider turning this into an enum so it's easier to reason with
    return true;
}