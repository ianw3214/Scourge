#include <windows.h>

#include "shade/instance/instance.h"
#include "shade/module/state.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/game/world.h"

enum class FacingDirection {
    LEFT,
    RIGHT
};

// TODO: Extract base movement component, allow player controlled vs AI controlled
class MovementComponent : public Shade::Component
{
public:
    FacingDirection mFacing = FacingDirection::RIGHT;
    bool mWasMoving = false;
public:
    MovementComponent(Shade::Entity& EntityRef) : Component(EntityRef) {}
    void Update(float deltaSeconds) override {
        FacingDirection NewFacingDir = mFacing;
        bool bMoving = false;
        if (mEntityRef.GetBooleanEvent("move_up").mHeld)
        {
            mEntityRef.SetPositionY(mEntityRef.GetPositionY() + 200.f * deltaSeconds);
            bMoving = true;
        }
        if (mEntityRef.GetBooleanEvent("move_down").mHeld)
        {
            mEntityRef.SetPositionY(mEntityRef.GetPositionY() - 200.f * deltaSeconds);
            bMoving = true;
        }
        if (mEntityRef.GetBooleanEvent("move_right").mHeld)
        {
            mEntityRef.SetPositionX(mEntityRef.GetPositionX() + 200.f * deltaSeconds);
            bMoving = true;
            NewFacingDir = FacingDirection::RIGHT;
        }
        if (mEntityRef.GetBooleanEvent("move_left").mHeld)
        {
            mEntityRef.SetPositionX(mEntityRef.GetPositionX() - 200.f * deltaSeconds);
            bMoving = true;
            NewFacingDir = FacingDirection::LEFT;
        }
        // Update animation if any of the movement related states were changed
        if (NewFacingDir != mFacing || bMoving != mWasMoving)
        {
            mWasMoving = bMoving;
            mFacing = NewFacingDir;
            if (bMoving)
            {
                mEntityRef.GetCachedAnimatedSprite()->ChangeAnimationState(mFacing == FacingDirection::RIGHT ? "run_right" : "run_left");
            }
            else
            {
                mEntityRef.GetCachedAnimatedSprite()->ChangeAnimationState(mFacing == FacingDirection::RIGHT ? "idle_right" : "idle_left");
            }
        }
    }
};

class CustomGameWorld : public Shade::GameWorldModule
{
public:
    CustomGameWorld() : Shade::GameWorldModule()
    {
        // Initialize input mappings
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_W, "move_up");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_S, "move_down");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_A, "move_left");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_D, "move_right");
        SetEventsFromMapping(mInputMapping);

        // Initialize a test entity
        Shade::TilesheetInfo tileSheetInfo { 128, 128, 5, 4 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo;
        animStateInfo["idle_right"] = { 0, 3 };
        animStateInfo["idle_left"] = { 4, 7 };
        animStateInfo["run_right"] = { 8, 13 };
        animStateInfo["run_left"] = { 14, 19 };
        std::unique_ptr<Shade::Entity> TestEntity = std::make_unique<Shade::Entity>(*this);
        TestEntity->AddComponent(std::make_unique<Shade::AnimatedSpriteComponent>(*TestEntity.get(), 128.f, 128.f, "assets/textures/player.png", tileSheetInfo, animStateInfo, "idle_right"));
        TestEntity->SetPositionX(200.f);
        TestEntity->SetPositionY(200.f);
        TestEntity->AddComponent(std::make_unique<MovementComponent>(*TestEntity.get()));
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