#define NOMINMAX   
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "definitions.h"

#include "shade/instance/instance.h"
#include "shade/instance/service/provider.h"
#include "shade/module/state.h"
#include "shade/editor/editor.h"
#include "shade/editor/editorBase.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/command.h"
#include "shade/graphics/command/drawLine.h"
#include "shade/resource/manager.h"
#include "shade/logging/logService.h"

#include "components/ai/blackBoardComponent.h"
#include "components/ai/stateMachineAIComponent.h"
#include "components/combat/attackComponent.h"
#include "components/combat/healthComponent.h"
#include "components/combat/hitboxComponent.h"
#include "components/facingComponent.h"
#include "components/movement/moveComponent.h"
#include "components/movement/locomotionComponent.h"
#include "components/player/cameraFollowComponent.h"
#include "components/player/playerInputComponent.h"

#include "map/mapService.h"

#include "editor/mapEditor/mapEditor.h"

#include "debug/debugModule.h"
#include "debug/basicDebugComponent.h"

#include <vector>

// ======================================
// TODO: This is a pretty hacky system, figure out a better way to track this in the future
class PlayerRegistry {
public:
    static void CachePlayer(Shade::Entity* player) { sCachedPlayer = player; }
    static Shade::Entity* GetCachedPlayer() { return sCachedPlayer; }
private:
    static inline Shade::Entity* sCachedPlayer = nullptr;
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
        mInputMapping.AddControllerButtonEventMapping(Shade::ControllerButton::SHADE_BUTTON_B, "attack");
        mInputMapping.AddControllerButtonEventMapping(Shade::ControllerButton::SHADE_BUTTON_A, "roll");
        mInputMapping.AddAxisEventMapping(Shade::ControllerAxis::SHADE_AXIS_LEFTX, "move_h");
        mInputMapping.AddAxisEventMapping(Shade::ControllerAxis::SHADE_AXIS_LEFTY, "move_v");
        SetEventsFromMapping(mInputMapping);

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
        PlayerEntity->AddComponent(std::make_unique<PlayerInputComponent>());
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
        // AddEntity(std::move(TestKnight));

        InitializeWorldFromMap("assets/breach/maps/test.kv");
    }
private:
    // ======================================
    void InitializeWorldFromMap(const std::string& mapPath)
    {
        // TODO: More thought needs to be put into when/where the map service should get initialized
        //  - In its current state, the map service should also get unregistered when the game state is cleaned up
        Shade::ServiceProvider::GetCurrentProvider()->RegisterService(new MapService());
        MapService* mapService = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
        mapService->LoadMap(mapPath);
    }
};

class GameState : public Shade::State {
public:
    GameState(Shade::ServiceProvider& serviceProviderRef) : Shade::State() {
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
#ifdef BUILD_BREACH_EDITOR
    Shade::EditorConfiguration config;
    config.mEditors.emplace_back(std::make_unique<MapEditor>());
    MainGameInstance.SetState(std::make_unique<Shade::EditorState>(MainGameInstance, config));
#else
    MainGameInstance.SetState(std::make_unique<GameState>(MainGameInstance));
#endif
    MainGameInstance.Run();

    return 0;
}