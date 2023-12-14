#include <windows.h>

#include "shade/instance/instance.h"
#include "shade/module/state.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/game/world.h"

class TestComponent : public Shade::Component
{
public:
    TestComponent(Shade::Entity& EntityRef) : Component(EntityRef) {}
    void Update(float DeltaSeconds) override {
        if (mEntityRef.GetBooleanEvent("move_up").mHeld)
        {
            mEntityRef.SetPositionY(mEntityRef.GetPositionY() + 200.f * DeltaSeconds);
        }
        if (mEntityRef.GetBooleanEvent("move_down").mHeld)
        {
            mEntityRef.SetPositionY(mEntityRef.GetPositionY() - 200.f * DeltaSeconds);
        }
        if (mEntityRef.GetBooleanEvent("move_right").mHeld)
        {
            mEntityRef.SetPositionX(mEntityRef.GetPositionX() + 200.f * DeltaSeconds);
        }
        if (mEntityRef.GetBooleanEvent("move_left").mHeld)
        {
            mEntityRef.SetPositionX(mEntityRef.GetPositionX() - 200.f * DeltaSeconds);
        }
    }
};

class CustomGameWorld : public Shade::GameWorldModule
{
public:
    // TODO: This might have to be moved to its own initialization
    //  - since the modules can be created before the game instance is created
    //  - otherwise, we might have to defer the state/module creation to after instance init
    CustomGameWorld() : Shade::GameWorldModule()
    {
        // Initialize input mappings
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_W, "move_up");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_S, "move_down");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_A, "move_left");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_D, "move_right");
        SetEventsFromMapping(mInputMapping);

        // Initialize a test entity
        std::unique_ptr<Shade::Entity> TestEntity = std::make_unique<Shade::Entity>(*this);
        TestEntity->AddComponent(std::make_unique<Shade::SpriteComponent>(*TestEntity.get(), 200.f, 200.f, "assets/textures/tiles.png"));
        TestEntity->SetPositionX(200.f);
        TestEntity->SetPositionY(200.f);
        TestEntity->AddComponent(std::make_unique<TestComponent>(*TestEntity.get()));
        AddEntity(std::move(TestEntity));
    }
};

class GameState : public Shade::State {
public:
    GameState(Shade::ServiceProvider& ServiceProviderRef) : Shade::State() {
        AddModule(std::make_unique<CustomGameWorld>());
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    Shade::GameInstance MainGameInstance;
    MainGameInstance.SetState(std::make_unique<GameState>(MainGameInstance));
    MainGameInstance.Run();

    return 0;
}