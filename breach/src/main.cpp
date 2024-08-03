#define NOMINMAX   
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "definitions.h"

#include "shade/instance/instance.h"
#include "shade/instance/service/provider.h"
#include "shade/module/state.h"
#include "shade/editor/editor.h"
#include "shade/editor/editorBase.h"
#include "shade/game/entity/factory.h"
#include "shade/game/entity/component/component.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/entity.h"
#include "shade/game/entity/serialization/loader.h"
#include "shade/game/world.h"
#include "shade/graphics/anim/animationFrameData.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/command.h"
#include "shade/graphics/command/drawLine.h"
#include "shade/graphics/texture.h"
#include "shade/resource/manager.h"
#include "shade/logging/logService.h"

#include "components/ai/blackBoardComponent.h"
#include "components/ai/stateMachineAIComponent.h"
#include "components/combat/attackComponent.h"
#include "components/combat/deathHandlingComponent.h"
#include "components/combat/healthComponent.h"
#include "components/combat/hitboxComponent.h"
#include "components/facingComponent.h"
#include "components/movement/locomotionComponent.h"
#include "components/movement/moveComponent.h"
#include "components/movement/staggerComponent.h"
#include "components/player/cameraFollowComponent.h"
#include "components/player/mapTransitionComponent.h"
#include "components/player/playerInputComponent.h"

#include "map/mapService.h"

#include "editor/mapEditor/mapEditor.h"

#include "debug/debugModule.h"
#include "debug/basicDebugComponent.h"

#include <vector>

// TODO: TEMP DEBUG
#include "shade/file/fileSystem.h"

// TODO: Hacky system, need to replace
#include "playerRegistry.h"

class CustomGameWorld : public Shade::GameWorldModule
{
public:
    // ======================================
    CustomGameWorld() : Shade::GameWorldModule()
    {
        // Initialize input mappings
        //  - Is this the right place to initialize this? Perhaps needs a bit more thought...
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

        InitializeWorldFromMap("assets/breach/maps/interior.kv");
        
        Debug_AddEntities();
    }
private:
    // ======================================
    // TODO: This should be replaced by an actual serialized entity loading system
    void Debug_AddEntities() {
        // TODO: Once entity loading is more properly setup then this doesn't need to be as hard-coded
        MapService* mapService = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
        Shade::Vec2 start = mapService->GetLayout().GetPlayerStart();

        // TODO: All of the following loading should be done by loading from a file
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
        playerAttack->RegisterAttackInfo("attack_right", AttackInfo("attack_right", true, 0.25f, AttackHitInfo(21, 40.f, AttackTarget::ENEMY, { AttackHitBox(0.f, 30.f, 98.f, 90.f)} )));
        playerAttack->RegisterAttackInfo("attack_left", AttackInfo("attack_left", true, 0.25f, AttackHitInfo(24, 40.f, AttackTarget::ENEMY, { AttackHitBox(-98.f, 30.f, 98.f, 90.f)} )));
        playerAttack->RegisterAttackInfo("dash_left", AttackInfo("roll_left", true, true, 0.4f, 800.f));
        playerAttack->RegisterAttackInfo("dash_right", AttackInfo("roll_right", true, true, 0.4f, 800.f));
        PlayerEntity->AddComponent(std::move(playerAttack));
        // TODO: Temp hacky code - find better fix
        //  - quick fix will be to just return the new component when a component is added
        //  - A 2 round initialization would be better: differentiate construction and initialization
        AttackComponent* playerAttackComp = PlayerEntity->GetComponent<AttackComponent>();
        playerAttackComp->RegisterAttacksToAnimFrames();
        PlayerEntity->SetPositionX(start.x);
        PlayerEntity->SetPositionY(start.y);
        PlayerEntity->AddComponent(std::make_unique<BaseMovementComponent>());
        PlayerEntity->AddComponent(std::make_unique<LocomotionComponent>(350.f));
        PlayerEntity->AddComponent(std::make_unique<FacingComponent>());
        PlayerEntity->AddComponent(std::make_unique<PlayerInputComponent>());
        PlayerEntity->AddComponent(std::make_unique<CameraFollowComponent>());
        PlayerEntity->AddComponent(std::make_unique<HealthComponent>(200.f));
        PlayerEntity->AddComponent(std::make_unique<HitboxComponent>(72.f, 128.f));
        PlayerEntity->AddComponent(std::make_unique<MapTransitionComponent>());
#ifdef DEBUG_BREACH
        PlayerEntity->AddComponent(std::make_unique<BasicDebugComponent>());
#endif
        std::unique_ptr<Shade::Entity>& NewPlayerRef = AddEntity(std::move(PlayerEntity));
        PlayerRegistry::CachePlayer(NewPlayerRef.get());

        Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
        Shade::ResourceHandle fireVFX = resourceManager->LoadResource<Shade::Texture>("assets/breach/VFX/fire.png");
        Shade::ResourceHandle knightAnimHandle = resourceManager->LoadResource<Shade::AnimationFrameData>("assets/textures/knight2.json");
        Shade::AnimationFrameData* knightAnim = resourceManager->GetResource<Shade::AnimationFrameData>(knightAnimHandle);

        // Testing a knight entity
        std::unique_ptr<Shade::Entity> TestKnight = std::make_unique<Shade::Entity>(*this, *this);
        std::unique_ptr<Shade::AnimatedSpriteComponent> enemySprite = std::make_unique<Shade::AnimatedSpriteComponent>(480.f, 420.f, "assets/textures/knight2.png", knightAnim, "IDLE_LEFT", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE);
        enemySprite->SetAnimationTransition("ATTACK_LEFT", "IDLE_LEFT");
        enemySprite->SetAnimationTransition("ATTACK_RIGHT", "IDLE_RIGHT");
        enemySprite->SetAnimationTransition("SPECIAL_CHARGE", "SPECIAL_HOLD");
        enemySprite->SetAnimationTransition("DIE_LEFT", "DEAD_LEFT");
        enemySprite->SetAnimationTransition("DIE_RIGHT", "DEAD_RIGHT");
        TestKnight->AddComponent(std::move(enemySprite));
        std::unique_ptr<AttackComponent> enemyAttack = std::make_unique<AttackComponent>();
        enemyAttack->RegisterAttackInfo("attack_right", AttackInfo("ATTACK_RIGHT", true, 0.7f, AttackHitInfo(16, 30.f, AttackTarget::PLAYER, { AttackHitBox(100.f, 0.f, 140.f, 200.f)} )));
        enemyAttack->RegisterAttackInfo("attack_left", AttackInfo("ATTACK_LEFT", true, 0.7f, AttackHitInfo(12, 30.f, AttackTarget::PLAYER, { AttackHitBox(-240.f, 0.f, 140.f, 200.f)} )));
        AttackInfo& specialAttack = enemyAttack->RegisterAttackInfo("special", AttackInfo("SPECIAL_CHARGE", true, 1.2f, AttackHitInfo(24, 30.f, AttackTarget::PLAYER, { AttackHitBox(-140.f, 0.f, 80.f, 80.f, fireVFX, 0.f, 0.f), AttackHitBox(60.f, 0.f, 80.f, 80.f, fireVFX, 0.f, 0.f), AttackHitBox(-40.f, 60.f, 80.f, 80.f, fireVFX, 0.f, 0.f), AttackHitBox(-40.f, -60.f, 80.f, 80.f, fireVFX, 0.f, 0.f) } )));
        // TODO: Currently this second triggers multiple times since the animation loops, need to figure out a better fix for this...
        specialAttack.mHitInfo.emplace_back(AttackHitInfo(25, 30.f, AttackTarget::PLAYER, { AttackHitBox(-200.f, 0.f, 80.f, 80.f, fireVFX, 0.f, 0.f), AttackHitBox(120.f, 0.f, 80.f, 80.f, fireVFX, 0.f, 0.f), AttackHitBox(-40.f, 100.f, 80.f, 80.f, fireVFX, 0.f, 0.f), AttackHitBox(-40.f, -100.f, 80.f, 80.f, fireVFX, 0.f, 0.f) } ));
        TestKnight->AddComponent(std::move(enemyAttack));
        // TODO: Temp hacky code - find better fix
        AttackComponent* enemyAttackComp = TestKnight->GetComponent<AttackComponent>();
        enemyAttackComp->RegisterAttacksToAnimFrames();
        TestKnight->SetPositionX(-100.f);
        TestKnight->SetPositionY(100.f);
        TestKnight->AddComponent(std::make_unique<BaseMovementComponent>());
        TestKnight->AddComponent(std::make_unique<LocomotionComponent>("IDLE_LEFT", "IDLE_RIGHT", "WALK_LEFT", "WALK_RIGHT"));
        TestKnight->AddComponent(std::make_unique<FacingComponent>());
        TestKnight->AddComponent(std::make_unique<HealthComponent>(300.f));
        TestKnight->AddComponent(std::make_unique<DeathHandlingComponent>("DIE_LEFT", "DIE_RIGHT"));
        TestKnight->AddComponent(std::make_unique<HitboxComponent>(120.f, 240.f));
        TestKnight->AddComponent(std::make_unique<StaggerComponent>("STAGGER_LEFT", "STAGGER_RIGHT"));

        // AI state machine definition
        // TODO: Move temp cooldown code into blackboard or something similar
        static float attackCooldown = 0.f;
        static float specialCooldown = 0.f;
        AIState idleState, moveState, attackState, specialState;
        idleState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
            if (attackCooldown > 0.f)
            {
                attackCooldown -= deltaSeconds;
            }
            if (specialCooldown > 0.f)
            {
                specialCooldown -= deltaSeconds;
            }
        };
        idleState.mTransitions.push_back([](Shade::Entity* AIEntity){ 
            StaggerComponent* stagger = AIEntity->GetComponent<StaggerComponent>();
            if (stagger && stagger->IsStaggering())
            {
                return "";
            }

            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            const float diff_x = AIEntity->GetPositionX() - player->GetPositionX();
            const float diff_y = AIEntity->GetPositionY() - player->GetPositionY();
            if ((diff_x * diff_x + diff_y * diff_y) > 40000.f )
            {
                return "move";
            }
            return specialCooldown > 0.f ? (attackCooldown > 0.f ? "" : "attack") : "special";
        });
        moveState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
            if (attackCooldown > 0.f)
            {
                attackCooldown -= deltaSeconds;
            }
            if (specialCooldown > 0.f)
            {
                specialCooldown -= deltaSeconds;
            }

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
            StaggerComponent* stagger = AIEntity->GetComponent<StaggerComponent>();
            if (stagger && stagger->IsStaggering())
            {
                return "";
            }

            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            const float diff_x = AIEntity->GetPositionX() - player->GetPositionX();
            const float diff_y = AIEntity->GetPositionY() - player->GetPositionY();
            return (diff_x * diff_x + diff_y * diff_y) < 40000.f ? (specialCooldown > 0.f ? (attackCooldown > 0.f ? "idle" : "attack") : "special") : "";
        });
        attackState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
        };
        attackState.mOnEnter = [](Shade::Entity* AIEntity){
            attackCooldown = 1.5f;

            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();

            const bool isRight = player->GetPositionX() > AIEntity->GetPositionX();
            bool attacked = attackComponent->TryDoAttack(isRight ? "attack_right" : "attack_left");
            if (attacked)
            {
                FacingComponent* facing = AIEntity->GetComponent<FacingComponent>();
                facing->mDirection = isRight ? FacingDirection::RIGHT : FacingDirection::LEFT;
            }
        };
        attackState.mTransitions.push_back([](Shade::Entity* AIEntity) {
            AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();
            return attackComponent->IsDoingAttack() ? "" : "idle";
        });
        specialState.mUpdate = [](Shade::Entity* AIEntity, float deltaSeconds) {
        };
        specialState.mOnEnter = [](Shade::Entity* AIEntity){
            specialCooldown = 1.5f;

            Shade::Entity* player = PlayerRegistry::GetCachedPlayer();
            AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();

            bool attacked = attackComponent->TryDoAttack("special");
        };
        specialState.mTransitions.push_back([](Shade::Entity* AIEntity) {
            AttackComponent* attackComponent = AIEntity->GetComponent<AttackComponent>();
            return attackComponent->IsDoingAttack() ? "" : "idle";
        });
        std::unordered_map<std::string, AIState> stateInfo;
        stateInfo["idle"] = idleState;
        stateInfo["move"] = moveState;
        stateInfo["attack"] = attackState;
        stateInfo["special"] = specialState;
        TestKnight->AddComponent(std::make_unique<StateMachineAIComponent>("idle", stateInfo));
        TestKnight->AddComponent(std::make_unique<BlackboardComponent>());
#ifdef DEBUG_BREACH
        TestKnight->AddComponent(std::make_unique<BasicDebugComponent>());
#endif
        // AddEntity(std::move(TestKnight));

        // Testing entity loading
        std::unique_ptr<Shade::Entity>& testKnight2 = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityFactory>()->CreateAndRegisterNewEntity("assets/breach/knight.kv");
        testKnight2->AddComponent(std::make_unique<BasicDebugComponent>());
    }
    // ======================================
    void InitializeWorldFromMap(const std::string& mapPath)
    {
        // TODO: More thought needs to be put into when/where the map service should get initialized
        //  - In its current state, the map service should also get unregistered when the game state is cleaned up
        Shade::ServiceProvider::GetCurrentProvider()->RegisterService(new MapService());
        MapService* mapService = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
        mapService->LoadMap(mapPath);
    }
    // ======================================
    virtual void Update(float deltaSeconds) override
    {
        GameWorldModule::Update(deltaSeconds);
        
        MapService* mapService = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
        if (mapService->HasQueuedMap())
        {
            // Clear existing entities first
            ClearAllEntities();
            mapService->LoadQueuedMap();
            Debug_AddEntities();
        }
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
    Shade::EditorService* editorService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EditorService>();
    editorService->RegisterEditor(std::make_unique<MapEditor>());
    MainGameInstance.SetState(std::make_unique<Shade::EditorState>(MainGameInstance));

    editorService->SetRunGameCallback([&MainGameInstance](){
        Shade::StateChangeService* stateChanger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::StateChangeService>();
        std::unique_ptr<GameState> gameState = std::make_unique<GameState>(MainGameInstance);
        gameState->AddModule(std::make_unique<Shade::EditorContextModule>());
        stateChanger->SetNextState(std::move(gameState));
    });
    editorService->SetStopGameCallback([&MainGameInstance](){
        Shade::StateChangeService* stateChanger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::StateChangeService>();
        stateChanger->SetNextState(std::make_unique<Shade::EditorState>(MainGameInstance));
    });
#else
    MainGameInstance.SetState(std::make_unique<GameState>(MainGameInstance));
#endif

    // Register game-specific components
    Shade::EntityLoaderService* entityLoader = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityLoaderService>();
    entityLoader->RegisterComponentLoader("movement", [](auto handle){ return new BaseMovementComponent(); });
    entityLoader->RegisterComponentLoader("facing", [](auto handle){ return new FacingComponent(); });
    entityLoader->RegisterComponentLoader("health", [](auto handle){ return HealthComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader("locomotion", [](auto handle){ return LocomotionComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader("death_handler", [](auto handle){ return DeathHandlingComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader("hitbox", [](auto handle){ return HitboxComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader("stagger", [](auto handle){ return StaggerComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader("attack", [](auto handle){ return AttackComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader("ai", [](auto handle){ return StateMachineAIComponent::LoadFromFileHandle(handle); });

    MainGameInstance.Run();

    return 0;
}