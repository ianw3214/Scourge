#include "world.h"

#include "shade/input/event.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"

class TestComponent : public Shade::Component
{
public:
    TestComponent(Shade::Entity& EntityRef) : Component(EntityRef) {}
    void Update(float DeltaSeconds) override {
        if (mEntityRef.GetBooleanEvent("test").mHeld)
        {
            mEntityRef.SetPositionX(mEntityRef.GetPositionX() + 0.1f);
        }
    }
};

// ======================================
Shade::GameWorldModule::GameWorldModule() = default;

// ======================================
Shade::GameWorldModule::~GameWorldModule() = default;

// ======================================
void Shade::GameWorldModule::Update(float deltaSeconds)
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;
        // Initialize input mappings
        mInputMapping.AddKeyEventMapping(KeyCode::SHADE_KEY_SPACE, "test");
        SetEventsFromMapping(mInputMapping);

        // Initialize a test entity
        std::unique_ptr<Entity> TestEntity = std::make_unique<Entity>(*this);
        TestEntity->AddComponent(std::make_unique<SpriteComponent>(*TestEntity.get(), 200.f, 200.f, "assets/textures/tiles.png"));
        TestEntity->SetPositionX(200.f);
        TestEntity->SetPositionY(200.f);
        TestEntity->AddComponent(std::make_unique<TestComponent>(*TestEntity.get()));
        AddEntity(std::move(TestEntity));
    }
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