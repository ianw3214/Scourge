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
    for (std::unique_ptr<Entity>& CurrentEntity : mEntities)
    {
        CurrentEntity->Update(deltaSeconds);
    }
}

// ======================================
void Shade::GameWorldModule::Render(std::vector<std::unique_ptr<RenderCommand>>& CommandQueue)
{
    for (std::unique_ptr<Entity>& CurrentEntity : mEntities)
    {
        if (SpriteComponent* Sprite = CurrentEntity->GetCachedSpriteComponent())
        {
            CommandQueue.emplace_back(Sprite->CreateRenderCommand());
        }
    }
}

// ======================================
bool Shade::GameWorldModule::HandleEvent(const InputEvent& Event) 
{
    if (Event.mType == InputEventType::KEY)
    {
        const std::string GameEvent = mInputMapping.GetKeyEvent(Event.mKeyCode);
        if (Event.mKeyEvent == KeyEventType::PRESS)
        {
            StartBooleanEvent(GameEvent);
        }
        if (Event.mKeyEvent == KeyEventType::RELEASE)
        {
            StopBooleanEvent(GameEvent);
        }
    }
    return true;
}

// ======================================
void Shade::GameWorldModule::AddEntity(std::unique_ptr<Entity> NewEntity)
{
    mEntities.emplace_back(std::move(NewEntity));
}