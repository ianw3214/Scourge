#include <windows.h>

#include "shade/instance/instance.h"
#include "shade/module/state.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/graphics/camera/camera.h"

#include "components/healthComponent.h"

// ======================================
enum class RenderLayer : int {
    // Hacky solution
    BACKGROUND = -5,
    DEFAULT = 0
};

// ======================================
enum class FacingDirection {
    LEFT,
    RIGHT
};

// ======================================
class BaseMovementComponent : public Shade::Component
{
public:
    FacingDirection mFacing = FacingDirection::RIGHT;   // This should probably live somewhere outside the base movement component
    bool mWasMoving = false;
    // These variables are directly set to control the movement
    // TODO: Consider when to reset these variables, or if they should even be reset per frame?
    bool mMovingLeft = false;
    bool mMovingRight = false;
    bool mMovingUp = false;
    bool mMovingDown = false;
    // These are configuration variables to set movement properties
    float mSpeed = 200.f;
public:
    // ======================================
    BaseMovementComponent() = default;
    BaseMovementComponent(float speed) : mSpeed(speed) {}
    // ======================================
    void Update(float deltaSeconds) override {
        FacingDirection NewFacingDir = mFacing;
        bool bMoving = false;
        if (mMovingUp)
        {
            mEntityRef->SetPositionY(mEntityRef->GetPositionY() + mSpeed * deltaSeconds);
            bMoving = true;
        }
        if (mMovingDown)
        {
            mEntityRef->SetPositionY(mEntityRef->GetPositionY() - mSpeed * deltaSeconds);
            bMoving = true;
        }
        if (mMovingRight)
        {
            mEntityRef->SetPositionX(mEntityRef->GetPositionX() + mSpeed * deltaSeconds);
            bMoving = true;
            NewFacingDir = FacingDirection::RIGHT;
        }
        if (mMovingLeft)
        {
            mEntityRef->SetPositionX(mEntityRef->GetPositionX() - mSpeed * deltaSeconds);
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
                mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(mFacing == FacingDirection::RIGHT ? "run_right" : "run_left");
            }
            else
            {
                mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(mFacing == FacingDirection::RIGHT ? "idle_right" : "idle_left");
            }
        }
    }
};

// ======================================
class PlayerMoveControlComponenet : public Shade::Component
{
public:
    FacingDirection mFacing = FacingDirection::RIGHT;
    bool mWasMoving = false;
public:
    // ======================================
    void Update(float deltaSeconds) override {
        BaseMovementComponent* moveComponent = mEntityRef->GetComponent<BaseMovementComponent>();
        if (moveComponent == nullptr)
        {
            // TODO: Error here...
            return;
        }
        moveComponent->mMovingUp = mEntityRef->GetBooleanEvent("move_up").mHeld;
        moveComponent->mMovingDown = mEntityRef->GetBooleanEvent("move_down").mHeld;
        moveComponent->mMovingRight = mEntityRef->GetBooleanEvent("move_right").mHeld;
        moveComponent->mMovingLeft = mEntityRef->GetBooleanEvent("move_left").mHeld;
    }
};

// ======================================
class RandomMovementComponent : public Shade::Component
{
public:
    // This is not really random, but just switch between moving left/right
    float mMoveDelta = 0.f;
    bool mLeftRightToggle = false;
public:
    // ======================================
    void Update(float deltaSeconds) override {
        BaseMovementComponent* moveComponent = mEntityRef->GetComponent<BaseMovementComponent>();
        if (moveComponent == nullptr)
        {
            // TODO: Error here...
            return;
        }
        mMoveDelta += deltaSeconds;
        if (mMoveDelta > 4.f) 
        {
            mMoveDelta = 0.f;
            mLeftRightToggle = !mLeftRightToggle;
        }
        const bool moving = mMoveDelta > 1.5f;
        bool bMoving = false;
        moveComponent->mMovingRight = moving && mLeftRightToggle;
        moveComponent->mMovingLeft = moving && !mLeftRightToggle;
    }
};

// ======================================
class CameraFollowComponent : public Shade::Component
{
public:
    // ======================================
    void Update(float deltaSeconds) override {
        Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
        // TODO: y position update should come from a better place
        camera->SetCameraPosition(mEntityRef->GetPositionX(), 360.f);
    }
};

// ======================================
class HorizontalParallaxComponent : public Shade::Component
{
    public:
    // ======================================
    HorizontalParallaxComponent(float parallax) : mParallaxFactor(parallax) {}
    // ======================================
    void Update(float deltaSeconds) override {
        // Assume things with a parallax component just work from position x=0
        //  - if this needs to change, need to separate concept between world x/y and rendering x/y
        //  - or camera needs to somehow know to not use world x/y for parallax entities
        Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
        mEntityRef->SetPositionX(camera->GetCameraInfo().x - mParallaxFactor * (camera->GetCameraInfo().x));
        // Assume the sprite for parallax components always have anchor set to the bottom middle
        mEntityRef->SetPositionY(0.f);
    }
    private:
    float mParallaxFactor = 1.0f;
};

class CustomGameWorld : public Shade::GameWorldModule
{
public:
    // ======================================
    CustomGameWorld() : Shade::GameWorldModule()
    {
        // Initialize input mappings
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_W, "move_up");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_S, "move_down");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_A, "move_left");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_D, "move_right");
        SetEventsFromMapping(mInputMapping);

        // Initialize background images
        std::unique_ptr<Shade::Entity> TestBackground1 = std::make_unique<Shade::Entity>(*this);
        TestBackground1->AddComponent(std::make_unique<Shade::SpriteComponent>(1280.f, 720.f, "assets/textures/sky.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground1->AddComponent(std::make_unique<HorizontalParallaxComponent>(0.f));
        AddEntity(std::move(TestBackground1));

        std::unique_ptr<Shade::Entity> TestBackground2 = std::make_unique<Shade::Entity>(*this);
        TestBackground2->AddComponent(std::make_unique<Shade::SpriteComponent>(1280.f, 720.f, "assets/textures/clouds.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground2->AddComponent(std::make_unique<HorizontalParallaxComponent>(0.5f));
        AddEntity(std::move(TestBackground2));

        std::unique_ptr<Shade::Entity> TestBackground3 = std::make_unique<Shade::Entity>(*this);
        TestBackground3->AddComponent(std::make_unique<Shade::SpriteComponent>(1280.f, 720.f, "assets/textures/background2.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground3->AddComponent(std::make_unique<HorizontalParallaxComponent>(1.0f));
        AddEntity(std::move(TestBackground3));

        // Initialize a player entity
        Shade::TilesheetInfo tileSheetInfo { 128, 128, 5, 4 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo;
        animStateInfo["idle_right"] = { 0, 3 };
        animStateInfo["idle_left"] = { 4, 7 };
        animStateInfo["run_right"] = { 8, 13 };
        animStateInfo["run_left"] = { 14, 19 };
        std::unique_ptr<Shade::Entity> PlayerEntity = std::make_unique<Shade::Entity>(*this);
        PlayerEntity->AddComponent(std::make_unique<Shade::AnimatedSpriteComponent>(128.f, 128.f, "assets/textures/player.png", tileSheetInfo, animStateInfo, "idle_right", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE));
        PlayerEntity->SetPositionX(200.f);
        PlayerEntity->SetPositionY(200.f);
        PlayerEntity->AddComponent(std::make_unique<BaseMovementComponent>(350.f));
        PlayerEntity->AddComponent(std::make_unique<PlayerMoveControlComponenet>());
        PlayerEntity->AddComponent(std::make_unique<CameraFollowComponent>());
        PlayerEntity->AddComponent(std::make_unique<HealthComponent>(200.f));
        AddEntity(std::move(PlayerEntity));

        // Initialize a test entity
        Shade::TilesheetInfo tileSheetInfo2 { 160, 160, 3, 3 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo2;
        animStateInfo2["idle"] = { 0, 0 };
        animStateInfo2["run"] = { 1, 6 };
        std::unique_ptr<Shade::Entity> TestEntity = std::make_unique<Shade::Entity>(*this);
        TestEntity->AddComponent(std::make_unique<Shade::AnimatedSpriteComponent>(160.f, 160.f, "assets/textures/knight.png", tileSheetInfo2, animStateInfo2, "run", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestEntity->SetPositionX(300.f);
        TestEntity->SetPositionY(300.f);
        AddEntity(std::move(TestEntity));

        // Testing a knight entity
        Shade::TilesheetInfo tileSheetInfo3 { 480, 420, 5, 4 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo3;
        animStateInfo3["idle_left"] = { 0, 0 };
        animStateInfo3["idle_right"] = { 1, 1 };
        animStateInfo3["run_left"] = { 2, 5 };
        animStateInfo3["run_right"] = { 6, 9 };
        animStateInfo3["attack_left"] = { 10, 13 };
        animStateInfo3["attack_right"] = { 14, 17 };
        animStateInfo3["recharge_left"] = { 18, 18 };
        animStateInfo3["recharge_right"] = { 19, 19 };
        std::unique_ptr<Shade::Entity> TestKnight = std::make_unique<Shade::Entity>(*this);
        TestKnight->AddComponent(std::make_unique<Shade::AnimatedSpriteComponent>(480.f, 420.f, "assets/textures/knight2.png", tileSheetInfo3, animStateInfo3, "idle_left", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestKnight->SetPositionX(-100.f);
        TestKnight->SetPositionY(300.f);
        TestKnight->AddComponent(std::make_unique<BaseMovementComponent>());
        TestKnight->AddComponent(std::make_unique<RandomMovementComponent>());
        AddEntity(std::move(TestKnight));
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