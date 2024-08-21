#include "entityEditor.h"

#include <imgui/imgui.h>

#include "shade/file/fileSystem.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
#include "shade/game/entity/container.h"
#include "shade/game/entity/entity.h"
#include "shade/game/entity/factory.h"
#include "shade/game/entity/serialization/loader.h"
#include "shade/game/event/source.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/imgui/service.h"
#include "shade/graphics/imgui/window.h"
#include "shade/input/event.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
namespace EntityEditorConstants {
    // TODO: Specify where to find root "assets" folder and use relative paths to find files
    const std::string TempTargetFilePath = "assets/breach/entities/target.kv";
}

// ======================================
class EntityEditorWindow : public Shade::ImGuiWindow {
public:
    EntityEditorWindow(Shade::EntityEditor& entityEditorRef) : ImGuiWindow("Entity editor window"), mEntityEditorRef(entityEditorRef) {}
    void Draw() override {
        Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        Shade::FileSystem* fileSystem = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FileSystem>();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        ImGui::Begin("Entity Editor", nullptr, window_flags);

        if (ImGui::BeginMenuBar()) {
            // TODO: Look into "nativefiledialog" to actually hook up to file dialogs
            //  - https://github.com/mlabbe/nativefiledialog
            //  - For now, just always open/save to the same file and manually rename to work with different map files
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    mEntityEditorRef.CreateEmptyEntity();
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    mEntityEditorRef.OpenFile();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    mEntityEditorRef.SaveFile();
                }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {
                    // TODO: Implement
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();

            std::unique_ptr<Shade::Entity>& entityData = mEntityEditorRef.GetEntityData();
            if (entityData != nullptr)
            {
                if (ImGui::Button("Add Component"))
                {
                    ImGui::OpenPopup("select_component");
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete component"))
                {
                    // TODO: Eventually implement, not really important now...   
                }

                if (ImGui::BeginPopup("select_component"))
                {
                    Shade::EntityLoaderService* loader = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityLoaderService>();
                    std::unique_ptr<Shade::Component> newComponent = loader->ShowNewComponentPopup();
                    if (newComponent != nullptr)
                    {
                        if (!entityData->HasComoponent(newComponent->GetComponentID()))
                        {
                            entityData->AddComponent(std::move(newComponent));
                        }
                        else
                        {
                            logger->LogWarning(std::string("Component already exists on entity: ") + newComponent->GetComponentID());
                        }
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                entityData->ShowImguiDetails();
            }
        }

        ImGui::End();
    }
private:
    Shade::EntityEditor& mEntityEditorRef;
};

// ======================================
Shade::EntityEditor::EntityEditor()
    : Shade::EditorBase("Entity Editor")
{

}

// ======================================
void Shade::EntityEditor::OnEnter()
{
    Shade::ImGuiService* imguiService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ImGuiService>();
    imguiService->TryRegisterWindow(std::make_unique<EntityEditorWindow>(*this));

    // TODO: This should probably check for double registration? caused minor issue before
    // Entities need a "world" to exist in
    static GameplayEventSource emptyEventSource;
    static EntityContainer emptyEntityContainer;
    EntityFactory* entityFactory = ServiceProvider::GetCurrentProvider()->GetService<EntityFactory>();
    entityFactory->RegisterEntityWorldInfo({ emptyEventSource, emptyEntityContainer });

    // TODO: Fix camera position
}

// ======================================
void Shade::EntityEditor::OnExit()
{
    // TODO: The string used here might want to use a shared const to avoid deleting the wrong window
    Shade::ImGuiService* imguiService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ImGuiService>();
    imguiService->DeleteWindow("Entity editor window");

    EntityFactory* entityFactory = ServiceProvider::GetCurrentProvider()->GetService<EntityFactory>();
    entityFactory->UnRegisterEntityWorldInfo();
}

// ======================================
void Shade::EntityEditor::Update(float deltaSeconds)
{
    if (mEntityData)
    {
        if (AnimatedSpriteComponent* sprite = mEntityData->GetCachedAnimatedSprite())
        {
            sprite->Update(deltaSeconds);
        }
    }
}

// ======================================
void Shade::EntityEditor::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{
    // Render the sprite/animated sprite of the current loaded entity
    if (mEntityData)
    {
        if (AnimatedSpriteComponent* sprite = mEntityData->GetCachedAnimatedSprite())
        {
            sprite->AddRenderCommands(commandQueue);
        }
        else if (SpriteComponent* sprite = mEntityData->GetCachedSpriteComponent())
        {
            sprite->AddRenderCommands(commandQueue);
        }
    }
}

// ======================================
bool Shade::EntityEditor::HandleEvent(const Shade::InputEvent& event)
{
    // TODO: Generalize keyboard shortcuts to the base editor implementation
    if (event.mType == Shade::InputEventType::KEY)
    {
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_LCONTROL)
        {
            mControlPressed = event.mKeyEvent == Shade::KeyEventType::PRESS;
        }
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_N && mControlPressed)
        {
            if (event.mKeyEvent == Shade::KeyEventType::PRESS)
            {
                CreateEmptyEntity();
            }
        }
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_O && mControlPressed)
        {
            if (event.mKeyEvent == Shade::KeyEventType::PRESS)
            {
                OpenFile();
            }
        }
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_S && mControlPressed)
        {
            if (event.mKeyEvent == Shade::KeyEventType::PRESS)
            {
                SaveFile();
            }
        }
    }
    return false;
}

// ======================================
void Shade::EntityEditor::SetEntityData(std::unique_ptr<Entity> entityData)
{
    mEntityData = std::move(entityData);
}

// ======================================
std::unique_ptr<Shade::Entity>& Shade::EntityEditor::GetEntityData()
{
    return mEntityData;
}

// ======================================
void Shade::EntityEditor::CreateEmptyEntity()
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    std::unique_ptr<Shade::Entity> newEntity = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityFactory>()->CreateNewEntity();
    if (newEntity == nullptr)
    {
        logger->LogError(std::string("Failed to create new entity"));
    }
    else
    {
        logger->LogInfo("Created new entity"); 
        SetEntityData(std::move(newEntity));  
    }
}

// ======================================
void Shade::EntityEditor::OpenFile()
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    std::unique_ptr<Shade::Entity> entityData = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityFactory>()->CreateNewEntity(EntityEditorConstants::TempTargetFilePath);
    if (entityData == nullptr)
    {
        logger->LogError(std::string("Failed to open '") + EntityEditorConstants::TempTargetFilePath + '\'');
    }
    else
    {
        logger->LogInfo(std::string("Opened '") + entityData->GetName() + '\'');   
        SetEntityData(std::move(entityData));

        // When an entity is loaded, place the camera so the sprite is rendered at the center
        //  TODO: Need to handle all the other different sprite anchor cases
        SpriteComponent* sprite = mEntityData->GetCachedSpriteComponent();
        Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
        camera->SetCameraPosition(0.f, sprite->GetRenderHeight() / 2.f);
    }
}

// ======================================
void Shade::EntityEditor::SaveFile()
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    if (mEntityData == nullptr)
    {
        logger->LogError("Tried saving entity when no entity is open...");
        return;
    }
    bool saveResult = mEntityData->Save(EntityEditorConstants::TempTargetFilePath);
    if (saveResult)
    {
        logger->LogInfo(std::string("Saved entity to file: '") + mEntityData->GetName() + '\'');   
    }
    else
    {
        logger->LogError(std::string("Failed to save to file: '") + EntityEditorConstants::TempTargetFilePath + '\'');
    }
}
