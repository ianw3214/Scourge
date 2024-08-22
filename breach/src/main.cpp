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

        // Initialize a player entity
        std::unique_ptr<Shade::Entity>& PlayerEntity = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityFactory>()->CreateAndRegisterNewEntity("assets/breach/entities/player.kv");
        // TODO: Serializae the rest of these components
        //  - Or maybe not necessary? if always only attaches to the player...
        PlayerEntity->AddComponent(std::make_unique<PlayerInputComponent>());
        PlayerEntity->AddComponent(std::make_unique<CameraFollowComponent>());
        PlayerEntity->AddComponent(std::make_unique<MapTransitionComponent>());
#ifdef DEBUG_BREACH
        PlayerEntity->AddComponent(std::make_unique<BasicDebugComponent>());
#endif
        PlayerRegistry::CachePlayer(PlayerEntity.get());
        PlayerEntity->SetPositionX(start.x);
        PlayerEntity->SetPositionY(start.y);

        // Testing entity loading
        std::unique_ptr<Shade::Entity>& testKnight2 = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityFactory>()->CreateAndRegisterNewEntity("assets/breach/entities/knight.kv");
#ifdef DEBUG_BREACH
        testKnight2->AddComponent(std::make_unique<BasicDebugComponent>());
#endif
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