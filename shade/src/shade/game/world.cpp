#include "world.h"

#include "shade/input/event.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"

// ======================================
Shade::GameWorldModule::GameWorldModule() = default;

// ======================================
Shade::GameWorldModule::~GameWorldModule() = default;

// ======================================
void Shade::GameWorldModule::Update(float deltaSeconds)
{
    UpdateEventStates();
    for (std::unique_ptr<Entity>& entity : mEntities)
    {
        entity->Update(deltaSeconds);
    }
}

// ======================================
void Shade::GameWorldModule::Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue)
{
    for (std::unique_ptr<Entity>& entity : mEntities)
    {
        if (SpriteComponent* sprite = entity->GetCachedSpriteComponent())
        {
            commandQueue.emplace_back(sprite->CreateRenderCommand());
        }
    }
}

// ======================================
bool Shade::GameWorldModule::HandleEvent(const InputEvent& event) 
{
    if (event.mType == InputEventType::KEY)
    {
        if (mInputMapping.HasEventForKey(event.mKeyCode))
        {
            const std::string gameEvent = mInputMapping.GetKeyEvent(event.mKeyCode);
            if (event.mKeyEvent == KeyEventType::PRESS)
            {
                StartBooleanEvent(gameEvent);
            }
            if (event.mKeyEvent == KeyEventType::RELEASE)
            {
                StopBooleanEvent(gameEvent);
            }
        }
    }
    if (event.mType == InputEventType::CONTROLLER_BUTTON)
    {
        if (mInputMapping.HasEventForControllerButton(event.mControllerButton))
        {
            const std::string gameEvent = mInputMapping.GetControllerButtonEvent(event.mControllerButton);
            if (event.mControllerButtonEvent == ButtonEventType::PRESS)
            {
                StartBooleanEvent(gameEvent);
            }
            if (event.mControllerButtonEvent == ButtonEventType::RELEASE)
            {
                StopBooleanEvent(gameEvent);
            }
        }
    }
    if (event.mType == InputEventType::CONTROLLER_AXIS)
    {
        if (mInputMapping.HasEventForAxis(event.mControllerAxis))
        {
            const std::string gameEvent = mInputMapping.GetControllerAxisEvent(event.mControllerAxis);
            UpdateFloatEvent(gameEvent, event.mControllerValue);
        }
    }
    return true;
}
