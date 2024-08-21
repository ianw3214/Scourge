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
        std::unique_ptr<Shade::AnimatedSpriteComponent> playerSprite = std::make_unique<Shade::AnimatedSpriteComponent>(196.f, 128.f, "assets/breach/textures/player.png", tileSheetInfo, animStateInfo, "idle_right", static_cast<int>(RenderLayer::DEFAULT), Shade::RenderAnchor::BOTTOM_MIDDLE);
        PlayerEntity->AddComponent(std::move(playerSprite));
        std::unique_ptr<AttackComponent> playerAttack = std::make_unique<AttackComponent>();
        playerAttack->RegisterAttackInfo("attack_right", AttackInfo("attack_right", true, 0.25f, AttackHitInfo(21, 40.f, AttackTarget::ENEMY, { AttackHitBox(0.f, 30.f, 98.f, 90.f)} )));
        playerAttack->RegisterAttackInfo("attack_left", AttackInfo("attack_left", true, 0.25f, AttackHitInfo(24, 40.f, AttackTarget::ENEMY, { AttackHitBox(-98.f, 30.f, 98.f, 90.f)} )));
        playerAttack->RegisterAttackInfo("dash_left", AttackInfo("roll_left", true, true, 0.4f, 800.f));
        playerAttack->RegisterAttackInfo("dash_right", AttackInfo("roll_right", true, true, 0.4f, 800.f));
        PlayerEntity->AddComponent(std::move(playerAttack));
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

        // Testing entity loading
        std::unique_ptr<Shade::Entity>& testKnight2 = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityFactory>()->CreateAndRegisterNewEntity("assets/breach/knight.kv");
        testKnight2->AddComponent(std::make_unique<BasicDebugComponent>());
    }
    // ======================================
    void InitializeWorldFromMap(const std::string& mapPath)
    {
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

    // Register project specific services
    Shade::ServiceProvider::GetCurrentProvider()->RegisterService(new MapService());

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
    entityLoader->RegisterComponentLoader(BaseMovementComponent::ComponentID, [](auto handle){ return new BaseMovementComponent(); });
    entityLoader->RegisterComponentLoader(FacingComponent::ComponentID, [](auto handle){ return new FacingComponent(); });
    entityLoader->RegisterComponentLoader(HealthComponent::ComponentID, [](auto handle){ return HealthComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader(LocomotionComponent::ComponentID, [](auto handle){ return LocomotionComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader(DeathHandlingComponent::ComponentID, [](auto handle){ return DeathHandlingComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader(HitboxComponent::ComponentID, [](auto handle){ return HitboxComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader(StaggerComponent::ComponentID, [](auto handle){ return StaggerComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader(AttackComponent::ComponentID, [](auto handle){ return AttackComponent::LoadFromFileHandle(handle); });
    entityLoader->RegisterComponentLoader(StateMachineAIComponent::ComponentID, [](auto handle){ return StateMachineAIComponent::LoadFromFileHandle(handle); });

    MainGameInstance.Run();

    return 0;
}