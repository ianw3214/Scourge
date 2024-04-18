#define NOMINMAX   
#define WIN32_LEAN_AND_MEAN
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
#include "components/movement/moveComponent.h"
#include "components/movement/locomotionComponent.h"

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
class PlayerInputComponenet : public Shade::Component
{
public:
    // ======================================
    void Update(float deltaSeconds) override {
        LocomotionComponent* locomotion = mEntityRef->GetComponent<LocomotionComponent>();
        AttackComponent* attackComponent = mEntityRef->GetComponent<AttackComponent>();
        if (locomotion == nullptr)
        {
            Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
            logService->LogWarning("No locomotion component found on entity with PlayerInputComponent");
            return;
        }
        if (attackComponent == nullptr)
        {
            Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
            logService->LogWarning("No attack component found on entity with PlayerInputComponent");
            return;
        }
        FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>();
        if (mEntityRef->GetBooleanEvent("attack").mHeld)
        {
            // TODO: Have these account for input direction before accounting for attack direction
            attackComponent->TryDoAttack(facing->mDirection == FacingDirection::RIGHT ? "attack_right" : "attack_left");
            return;
        }
        if (mEntityRef->GetBooleanEvent("roll").mHeld)
        {
            // TODO: Have these account for input direction before accounting for attack direction
            attackComponent->TryDoAttack(facing->mDirection == FacingDirection::RIGHT ? "dash_right" : "dash_left");
            return;
        }
        locomotion->mMovingUp = mEntityRef->GetBooleanEvent("move_up").mHeld;
        locomotion->mMovingDown = mEntityRef->GetBooleanEvent("move_down").mHeld;
        locomotion->mMovingRight = mEntityRef->GetBooleanEvent("move_right").mHeld;
        locomotion->mMovingLeft = mEntityRef->GetBooleanEvent("move_left").mHeld;
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
        mInputMapping.AddKeyEventMapping(Shade::KeyCode::SHADE_KEY_SPACE, "roll");
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
        Shade::TilesheetInfo tileSheetInfo { 196, 128, 6, 6 };
        std::unordered_map<std::string, Shade::AnimationStateInfo> animStateInfo;
        animStateInfo["idle_right"] = { 0, 3 };
        animStateInfo["idle_left"] = { 4, 7 };
        animStateInfo["run_right"] = { 8, 13 };
        animStateInfo["run_left"] = { 14, 19 };
        animStateInfo["attack_right"] = { 20, 22, "idle_right" };
        animStateInfo["attack_left"] = { 23, 25, "idle_left" };
        animStateInfo["roll_right"] = { 26, 30, "idle_right" };
        animStateInfo["roll_left"] = { 31, 35, "idle_left" };
        std::unique_ptr<Shade::Entity> PlayerEntity = std::make_unique<Shade::Entity>(*this, *this);
        std::unique_ptr<Shade::AnimatedSpriteComponent> playerSprite = std::make_unique<Shade::AnimatedSpriteComponent>(196.f, 128.f, "assets/textures/player.png", tileSheetInfo, animStateInfo, "idle_right", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE);
        PlayerEntity->AddComponent(std::move(playerSprite));
        std::unique_ptr<AttackComponent> playerAttack = std::make_unique<AttackComponent>();
        playerAttack->RegisterAttackInfo("attack_right", AttackInfo("attack_right", true, 0.25f, AttackHitInfo(21, 0.f, 30.f, 98.f, 90.f, 10.f, AttackTarget::ENEMY)));
        playerAttack->RegisterAttackInfo("attack_left", AttackInfo("attack_left", true, 0.25f, AttackHitInfo(24, -98.f, 30.f, 98.f, 90.f, 10.f, AttackTarget::ENEMY)));
        playerAttack->RegisterAttackInfo("dash_left", AttackInfo("roll_left", true, true, 0.4f, 800.f));
        playerAttack->RegisterAttackInfo("dash_right", AttackInfo("roll_right", true, true, 0.4f, 800.f));
        PlayerEntity->AddComponent(std::move(playerAttack));
        // TODO: Temp hacky code - find better fix
        //  - quick fix will be to just return the new component when a component is added
        //  - A 2 round initialization would be better: differentiate construction and initialization
        AttackComponent* playerAttackComp = PlayerEntity->GetComponent<AttackComponent>();
        playerAttackComp->RegisterAttacksToAnimFrames();
        PlayerEntity->SetPositionX(200.f);
        PlayerEntity->SetPositionY(200.f);
        PlayerEntity->AddComponent(std::make_unique<BaseMovementComponent>());
        PlayerEntity->AddComponent(std::make_unique<LocomotionComponent>(350.f));
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
        std::unique_ptr<AttackComponent> enemyAttack = std::make_unique<AttackComponent>();
        enemyAttack->RegisterAttackInfo("attack_right", AttackInfo("attack_right", true, 1.4f, AttackHitInfo(16, 100.f, 0.f, 140.f, 200.f, 30.f, AttackTarget::PLAYER)));
        enemyAttack->RegisterAttackInfo("attack_left", AttackInfo("attack_left", true, 1.4f, AttackHitInfo(12, -240.f, 0.f, 140.f, 200.f, 30.f, AttackTarget::PLAYER)));
        TestKnight->AddComponent(std::move(enemyAttack));
        // TODO: Temp hacky code - find better fix
        AttackComponent* enemyAttackComp = TestKnight->GetComponent<AttackComponent>();
        enemyAttackComp->RegisterAttacksToAnimFrames();
        TestKnight->SetPositionX(-100.f);
        TestKnight->SetPositionY(100.f);
        TestKnight->AddComponent(std::make_unique<BaseMovementComponent>());
        TestKnight->AddComponent(std::make_unique<LocomotionComponent>());
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
            LocomotionComponent* locomotion = AIEntity->GetComponent<LocomotionComponent>();
            if (locomotion == nullptr)
            {
                Shade::LogService* logService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
                logService->LogError("Expected locomotion component for AI");
                return;
            }
            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            locomotion->mMovingRight = player->GetPositionX() > AIEntity->GetPositionX();
            locomotion->mMovingLeft = player->GetPositionX() < AIEntity->GetPositionX();
            locomotion->mMovingUp = player->GetPositionY() > AIEntity->GetPositionY();
            locomotion->mMovingDown = player->GetPositionY() < AIEntity->GetPositionY();
        };
        moveState.mTransitions.push_back([](Shade::Entity* AIEntity){
            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            const float diff_x = AIEntity->GetPositionX() - player->GetPositionX();
            const float diff_y = AIEntity->GetPositionY() - player->GetPositionY();
            return (diff_x * diff_x + diff_y * diff_y) < 40000.f ? "attack" : "";
        });
        attackState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
        };
        attackState.mOnEnter = [](Shade::Entity* AIEntity){
            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();
            attackComponent->TryDoAttack(player->GetPositionX() > AIEntity->GetPositionX() ? "attack_right" : "attack_left");
        };
        attackState.mTransitions.push_back([](Shade::Entity* AIEntity) {
            AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();
            return attackComponent->IsDoingAttack() ? "" : "idle";

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