#include "setColourMultiplier.h"

#include "shade/graphics/renderer.h"

// ======================================
Shade::SetColourMultiplierCommand::SetColourMultiplierCommand(Shade::Colour colour)
    : mColour(colour)
{

}

// ======================================
void Shade::SetColourMultiplierCommand::Execute(RendererBase* renderer, const CameraInfo& camera) 
{
    renderer->SetColourMultiplier(mColour);
}

// ======================================
Shade::ResetColourMultiplierCommand::ResetColourMultiplierCommand()
{

}

// ======================================
void Shade::ResetColourMultiplierCommand::Execute(RendererBase* renderer, const CameraInfo& camera) 
{
    renderer->ResetColourMultiplier();
}