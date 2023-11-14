#include "world.h"

#include "shade/graphics/command/drawRectangle.h"
#include "shade/graphics/command/drawLine.h"
#include "shade/graphics/command/drawTexture.h"

// ======================================
Shade::GameWorldModule::~GameWorldModule()
{

}

// ======================================
void Shade::GameWorldModule::Update(float deltaSeconds)
{
    
}

// ======================================
void Shade::GameWorldModule::Render(std::vector<std::unique_ptr<RenderCommand>>& CommandQueue)
{
    CommandQueue.emplace_back(std::make_unique<DrawLineCommand>(0.f, 0.f, 1.f, 1.f, Colour{1.f, 0.f, 1.f}, true));
    CommandQueue.emplace_back(std::make_unique<DrawRectangleCommand>(-1.f, -1.f, 1.f, 1.f, Colour{0.f, 1.f, 0.f}, true));
    CommandQueue.emplace_back(std::make_unique<DrawTextureCommand>(-1.f, 0.f, 1.f, 1.f, "assets/textures/tiles.png", true));
    CommandQueue.emplace_back(std::make_unique<DrawLineCommand>(0.f, 0.f, 1280.f, 360.f, Colour{0.f, 1.f, 1.f}));
    CommandQueue.emplace_back(std::make_unique<DrawRectangleCommand>(400.f, 400.f, 40.f, 40.f, Colour{0.5f, 0.1f, 0.1f}));
    CommandQueue.emplace_back(std::make_unique<DrawTextureCommand>(800.f, 300.f, 100.f, 100.f, "assets/textures/tiles.png"));
}