#include <windows.h>

#include "shade/instance/instance.h"
#include "shade/module/state.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/command.h"
#include "shade/graphics/command/drawLine.h"
#include "shade/logging/logService.h"

#include "components/ai/blackBoardComponent.h"
#include "components/ai/stateMachineAIComponent.h"
#include "components/attackComponent.h"
#include "components/facingComponent.h"
#include "components/healthComponent.h"
#include "components/hitboxComponent.h"

#include "debug/debugModule.h"
#include "debug/basicDebugComponent.h"

#include <vector>

// ======================================
enum class RenderLayer : int {
    // Hacky solution
    BACKGROUND = -5,
    DEFAULT = 0
};

// ======================================
// TODO: This is a pretty hacky system, figure out a better way to track this in the future
class PlayerRegistry {
public:
    static void CachePlayer(Shade::Entity* player) { sCachedPlayer = player; }
    static Shade::Entity* GetCachedPlayer() { return sCachedPlayer; }
private:
    static inline Shade::Entity* sCachedPlayer = nullptr;
};

// ======================================
class BaseMovementComponent : public Shade::Component
{
public:
    bool mWasMoving = false;
    bool mDisable = false;
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
        FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
        if (mDisable)
        {
            return;
        }
        FacingDirection NewFacingDir = facing->mDirection;
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
        if (NewFacingDir != facing->mDirection || bMoving != mWasMoving)
        {
            mWasMoving = bMoving;
            facing->mDirection = NewFacingDir;
            if (bMoving)
            {
                mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "run_right" : "run_left");
            }
            else
            {
                mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "idle_right" : "idle_left");
            }
        }
        // Reset movement flags after using them
        mMovingLeft = false;
        mMovingRight = false;
        mMovingUp = false;
        mMovingDown = false;
    }

    void DisableMovement(bool disable)
    {
        mDisable = disable;
        mWasMoving = false;  // TODO: Hacky... fix soon
    }
};

// ======================================
class PlayerInputComponenet : public Shade::Component
{
public:
    // Attack info
    // TODO: This should be in a separate component
    float mAttackTimer = 0.f;
public:
    // ======================================
    void Update(float deltaSeconds) override {
        BaseMovementComponent* moveComponent = mEntityRef->GetComponent<BaseMovementComponent>();
        if (moveComponent == nullptr)
        {
            Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
            logService->LogWarning("No movement component found on entity with PlayerInputComponent");
            return;
        }
        // TODO: Probably want to use a state machine to keep track of this
        //  - Alternatively, some sort of "control flag" system where components can try to get control of a certain flag (e.g. movement flag)
        //  - If the flag is in use, then don't allow movement to go through
        //  - Some sort of priority can be used to resolve situations where multiple components try to control a flag at the same time
        if (mAttackTimer > 0.f) {
            mAttackTimer -= deltaSeconds;
            if (mAttackTimer <= 0.f)
            {
                moveComponent->mDisable = false;
            }
            return;
        }
        if (mEntityRef->GetBooleanEvent("attack").mHeld)
        {
            FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
            mEntityRef->GetCachedAnimatedSprite()->ChangeAnimationState(facing->mDirection == FacingDirection::RIGHT ? "attack_right" : "attack_left");
            mAttackTimer = 0.25f;
            moveComponent->DisableMovement(true);
            return;
        }
        moveComponent->mMovingUp = mEntityRef->GetBooleanEvent("move_up").mHeld;
        moveComponent->mMovingDown = mEntityRef->GetBooleanEvent("move_down").mHeld;
        moveComponent->mMovingRight = mEntityRef->GetBooleanEvent("move_right").mHeld;
        moveComponent->mMovingLeft = mEntityRef->GetBooleanEvent("move_left").mHeld;
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
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_UP, "move_up");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_DOWN, "move_down");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_LEFT, "move_left");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_RIGHT, "move_right");
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_Z, "attack");
        SetEventsFromMapping(mInputMapping);

        // Initialize background images
        std::unique_ptr<Shade::Entity> TestBackground1 = std::make_unique<Shade::Entity>(*this, *this);
        TestBackground1->AddComponent(std::make_unique<Shade::SpriteComponent>(1280.f, 720.f, "assets/breach/bg.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground1->AddComponent(std::make_unique<HorizontalParallaxComponent>(0.f));
        AddEntity(std::move(TestBackground1));

        std::unique_ptr<Shade::Entity> TestBackground2 = std::make_unique<Shade::Entity>(*this, *this);
        TestBackground2->AddComponent(std::make_unique<Shade::SpriteComponent>(2000.f, 720.f, "assets/breach/clouds.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground2->AddComponent(std::make_unique<HorizontalParallaxComponent>(0.5f));
        AddEntity(std::move(TestBackground2));

        std::unique_ptr<Shade::Entity> TestBackground3 = std::make_unique<Shade::Entity>(*this, *this);
        TestBackground3->AddComponent(std::make_unique<Shade::SpriteComponent>(2400.f, 720.f, "assets/breach/mid.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground3->AddComponent(std::make_unique<HorizontalParallaxComponent>(0.7f));
        AddEntity(std::move(TestBackground3));

        std::unique_ptr<Shade::Entity> TestBackground4 = std::make_unique<Shade::Entity>(*this, *this);
        TestBackground4->AddComponent(std::make_unique<Shade::SpriteComponent>(2800.f, 720.f, "assets/breach/foreground.png", static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestBackground4->AddComponent(std::make_unique<HorizontalParallaxComponent>(1.0f));
        AddEntity(std::move(TestBackground4));

        // Initialize a player entity
        Shade::TilesheetInfo tileSheetInfo { 196, 128, 6, 5 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo;
        animStateInfo["idle_right"] = { 0, 3 };
        animStateInfo["idle_left"] = { 4, 7 };
        animStateInfo["run_right"] = { 8, 13 };
        animStateInfo["run_left"] = { 14, 19 };
        animStateInfo["attack_right"] = { 20, 22, "idle_right" };
        animStateInfo["attack_left"] = { 23, 25, "idle_left" };
        std::unique_ptr<Shade::Entity> PlayerEntity = std::make_unique<Shade::Entity>(*this, *this);
        std::unique_ptr<Shade::AnimatedSpriteComponent> playerSprite = std::make_unique<Shade::AnimatedSpriteComponent>(196.f, 128.f, "assets/textures/player.png", tileSheetInfo, animStateInfo, "idle_right", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE);
        PlayerEntity->AddComponent(std::move(playerSprite));
        std::unique_ptr<AttackComponent> playerAttack = std::make_unique<AttackComponent>();
        playerAttack->RegisterAttackInfo("attack_right", AttackInfo(0.f, 30.f, 98.f, 90.f, 10.f));
        playerAttack->RegisterAttackInfo("attack_left", AttackInfo(-98.f, 30.f, 98.f, 90.f, 10.f));
        PlayerEntity->AddComponent(std::move(playerAttack));
        // TODO: Temp hacky code - find better fix
        AttackComponent* playerAttackComp = PlayerEntity->GetComponent<AttackComponent>();
        playerAttackComp->RegisterAttacksToAnimFrames({ std::make_pair<>("attack_right", 21), std::make_pair<>("attack_left", 24) });
        PlayerEntity->SetPositionX(200.f);
        PlayerEntity->SetPositionY(200.f);
        PlayerEntity->AddComponent(std::make_unique<BaseMovementComponent>(350.f));
        PlayerEntity->AddComponent(std::make_unique<FacingComponent>());
        PlayerEntity->AddComponent(std::make_unique<PlayerInputComponenet>());
        PlayerEntity->AddComponent(std::make_unique<CameraFollowComponent>());
        PlayerEntity->AddComponent(std::make_unique<HealthComponent>(200.f));
        PlayerEntity->AddComponent(std::make_unique<HitboxComponent>(72.f, 128.f));
#ifdef DEBUG_BREACH
        PlayerEntity->AddComponent(std::make_unique<BasicDebugComponent>());
#endif
        std::unique_ptr<Shade::Entity>& NewPlayerRef = AddEntity(std::move(PlayerEntity));
        PlayerRegistry::CachePlayer(NewPlayerRef.get());

        // Testing a knight entity
        Shade::TilesheetInfo tileSheetInfo3 { 480, 420, 5, 4 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo3;
        animStateInfo3["idle_left"] = { 0, 0 };
        animStateInfo3["idle_right"] = { 1, 1 };
        animStateInfo3["run_left"] = { 2, 5 };
        animStateInfo3["run_right"] = { 6, 9 };
        animStateInfo3["attack_left"] = { 10, 13, "idle_left" };
        animStateInfo3["attack_right"] = { 14, 17, "idle_right" };
        animStateInfo3["recharge_left"] = { 18, 18 };
        animStateInfo3["recharge_right"] = { 19, 19 };
        std::unique_ptr<Shade::Entity> TestKnight = std::make_unique<Shade::Entity>(*this, *this);
        TestKnight->AddComponent(std::make_unique<Shade::AnimatedSpriteComponent>(480.f, 420.f, "assets/textures/knight2.png", tileSheetInfo3, animStateInfo3, "idle_left", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE));
        TestKnight->SetPositionX(-100.f);
        TestKnight->SetPositionY(100.f);
        TestKnight->AddComponent(std::make_unique<BaseMovementComponent>());
        TestKnight->AddComponent(std::make_unique<FacingComponent>());
        TestKnight->AddComponent(std::make_unique<HealthComponent>(300.f));
        TestKnight->AddComponent(std::make_unique<HitboxComponent>(120.f, 240.f));

        // AI state machine definition
        AIState idleState, moveState, attackState;
        idleState.mTransitions.push_back([](Shade::Entity* AIEntity){ 
            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            const float diff_x = AIEntity->GetPositionX() - player->GetPositionX();
            const float diff_y = AIEntity->GetPositionY() - player->GetPositionY();
            return (diff_x * diff_x + diff_y * diff_y) < 1000000.f ? "move" : "";
        });
        moveState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
            BaseMovementComponent* moveComponent = AIEntity->GetComponent<BaseMovementComponent>();
            if (moveComponent == nullptr)
            {
                // TODO: Error here...
                return;
            }
            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            moveComponent->mMovingRight = player->GetPositionX() > AIEntity->GetPositionX();
            moveComponent->mMovingLeft = player->GetPositionX() < AIEntity->GetPositionX();
            moveComponent->mMovingUp = player->GetPositionY() > AIEntity->GetPositionY();
            moveComponent->mMovingDown = player->GetPositionY() < AIEntity->GetPositionY();
        };
        moveState.mTransitions.push_back([](Shade::Entity* AIEntity){
            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            const float diff_x = AIEntity->GetPositionX() - player->GetPositionX();
            const float diff_y = AIEntity->GetPositionY() - player->GetPositionY();
            return (diff_x * diff_x + diff_y * diff_y) < 40000.f ? "attack" : "";
        });
        attackState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
            BlackboardComponent* blackboard = AIEntity->GetComponent<BlackboardComponent>();
            blackboard->StoreFloat("attack_timer", blackboard->GetFloat("attack_timer") - deltaSeconds);
        };
        attackState.mOnEnter = [](Shade::Entity* AIEntity){
            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            BaseMovementComponent* moveComponent = AIEntity->GetComponent<BaseMovementComponent>();
            //  - TODO: Consider if this is the right place to put movement disabling
            //  - Perhaps it makes more sense as a "on transition out" for the move state
            moveComponent->DisableMovement(true);
            AIEntity->GetCachedAnimatedSprite()->ChangeAnimationState(player->GetPositionX() > AIEntity->GetPositionX() ? "attack_right" : "attack_left");

            BlackboardComponent* blackboard = AIEntity->GetComponent<BlackboardComponent>();
            blackboard->StoreFloat("attack_timer", 1.2f);
        };
        attackState.mTransitions.push_back([](Shade::Entity* AIEntity) {
            BlackboardComponent* blackboard = AIEntity->GetComponent<BlackboardComponent>();
            if (blackboard->GetFloat("attack_timer") <= 0.f)
            {
                // TODO: Perhaps there's a better place to put this...
                BaseMovementComponent* moveComponent = AIEntity->GetComponent<BaseMovementComponent>();
                moveComponent->mDisable = false;
                return "idle";
            }
            return "";
        });
        std::unordered_map<std::string, AIState> stateInfo;
        stateInfo["idle"] = idleState;
        stateInfo["move"] = moveState;
        stateInfo["attack"] = attackState;
        TestKnight->AddComponent(std::make_unique<StateMachineAIComponent>("idle", stateInfo));
        TestKnight->AddComponent(std::make_unique<BlackboardComponent>());
#ifdef DEBUG_BREACH
        TestKnight->AddComponent(std::make_unique<BasicDebugComponent>());
#endif
        AddEntity(std::move(TestKnight));
    }
};

class GameState : public Shade::State {
public:
    GameState(Shade::ServiceProvider& ServiceProviderRef) : Shade::State() {
        AddModule(std::make_unique<CustomGameWorld>());
#ifdef DEBUG_BREACH
        AddModule(std::make_unique<CustomDebugModule>());
#endif
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