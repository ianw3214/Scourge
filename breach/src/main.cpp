#include <windows.h>

#include "shade/instance/instance.h"
#include "shade/module/state.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/graphics/camera/camera.h"

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
// TODO: Extract base movement component, allow player controlled vs AI controlled
class MovementComponent : public Shade::Component
{
public:
    FacingDirection mFacing = FacingDirection::RIGHT;
    bool mWasMoving = false;
public:
    // ======================================
    MovementComponent(Shade::Entity& EntityRef) : Component(EntityRef) {}
    // ======================================
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

// ======================================
class CameraFollowComponent : public Shade::Component
{
public:
    // ======================================
    CameraFollowComponent(Shade::Entity& EntityRef) : Component(EntityRef) {}
    // ======================================
    void Update(float deltaSeconds) override {
        Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
        // TODO: y position update should come from a better place
        camera->SetCameraPosition(mEntityRef.GetPositionX(), 360.f);
    }
};

// ======================================
class HorizontalParallaxComponent : public Shade::Component
{
    public:
    // ======================================
    HorizontalParallaxComponent(Shade::Entity& EntityRef, float parallax) : Component(EntityRef), mParallaxFactor(parallax) {}
    // ======================================
    void Update(float deltaSeconds) override {
        // Assume things with a parallax component just work from position x=0
        //  - if this needs to change, need to separate concept between world x/y and rendering x/y
        //  - or camera needs to somehow know to not use world x/y for parallax entities
        Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
        mEntityRef.SetPositionX(camera->GetCameraInfo().x - mParallaxFactor * (camera->GetCameraInfo().x));
        // Assume the sprite for parallax components always have anchor set to the bottom middle
        mEntityRef.SetPositionY(0.f);
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
        TestBackground1->AddComponent(std::make_unique<Shade::SpriteComponent>(*TestBackground1.get(), 1280.f, 720.f, "assets/textures/sky.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground1->AddComponent(std::make_unique<HorizontalParallaxComponent>(*TestBackground1.get(), 0.f));
        AddEntity(std::move(TestBackground1));

        std::unique_ptr<Shade::Entity> TestBackground2 = std::make_unique<Shade::Entity>(*this);
        TestBackground2->AddComponent(std::make_unique<Shade::SpriteComponent>(*TestBackground2.get(), 1280.f, 720.f, "assets/textures/clouds.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground2->AddComponent(std::make_unique<HorizontalParallaxComponent>(*TestBackground2.get(), 0.5f));
        AddEntity(std::move(TestBackground2));

        std::unique_ptr<Shade::Entity> TestBackground3 = std::make_unique<Shade::Entity>(*this);
        TestBackground3->AddComponent(std::make_unique<Shade::SpriteComponent>(*TestBackground3.get(), 1280.f, 720.f, "assets/textures/background2.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground3->AddComponent(std::make_unique<HorizontalParallaxComponent>(*TestBackground3.get(), 1.0f));
        AddEntity(std::move(TestBackground3));

        // Initialize a test entity
        Shade::TilesheetInfo tileSheetInfo { 128, 128, 5, 4 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo;
        animStateInfo["idle_right"] = { 0, 3 };
        animStateInfo["idle_left"] = { 4, 7 };
        animStateInfo["run_right"] = { 8, 13 };
        animStateInfo["run_left"] = { 14, 19 };
        std::unique_ptr<Shade::Entity> TestEntity = std::make_unique<Shade::Entity>(*this);
        TestEntity->AddComponent(std::make_unique<Shade::AnimatedSpriteComponent>(*TestEntity.get(), 128.f, 128.f, "assets/textures/player.png", tileSheetInfo, animStateInfo, "idle_right", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestEntity->SetPositionX(200.f);
        TestEntity->SetPositionY(200.f);x
        TestEntity->AddComponent(std::make_unique<MovementComponent>(*TestEntity.get()));
        TestEntity->AddComponent(std::make_unique<CameraFollowComponent>(*TestEntity.get()));
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