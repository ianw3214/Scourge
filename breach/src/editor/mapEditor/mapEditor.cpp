#include "mapEditor.h"

#include <string>

#include <imgui/imgui.h>

#include "shade/file/fileSystem.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/graphics/common.h"
#include "shade/graphics/imgui/service.h"
#include "shade/graphics/imgui/window.h"
#include "shade/graphics/texture.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

#include "definitions.h"
#include "components/parallaxComponent.h"
#include "map/map.h"

namespace MapEditorConstants {
    // TODO: Specify where to find root "assets" folder and use relative paths to find files
    const std::string TempTargetFilePath = "assets/breach/maps/target.kv";
}

// ======================================
class EditorMapData : public MapData {
public:
    EditorMapData() : MapData("New map", {}, MapLayout{}) {}
};

// ======================================
class MapEditorWindow : public Shade::ImGuiWindow {
public:
    MapEditorWindow(MapEditor& mapEditorRef) : mMapEditorRef(mapEditorRef) {}
    void Draw() override {
        Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        Shade::FileSystem* fileSystem = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FileSystem>();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
        ImGui::Begin("Map Editor", nullptr, window_flags);

        if (ImGui::BeginMenuBar()) {
            // TODO: Look into "nativefiledialog" to actually hook up to file dialogs
            //  - https://github.com/mlabbe/nativefiledialog
            //  - For now, just always open/save to the same file and manually rename to work with different map files
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    MapData* newMapData = new EditorMapData();
                    if (newMapData == nullptr)
                    {
                        logger->LogError(std::string("Failed to open '") + MapEditorConstants::TempTargetFilePath + '\'');
                    }
                    else
                    {
                        logger->LogInfo(std::string("Created new map: '") + newMapData->GetName() + '\''); 
                        mMapEditorRef.SetMapData(std::unique_ptr<MapData>(newMapData));  
                    }
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    MapData* rawMapData = dynamic_cast<MapData*>(MapData::Load(MapEditorConstants::TempTargetFilePath));
                    if (rawMapData == nullptr)
                    {
                        logger->LogError(std::string("Failed to open '") + MapEditorConstants::TempTargetFilePath + '\'');
                    }
                    else
                    {
                        logger->LogInfo(std::string("Opened '") + rawMapData->GetName() + '\'');   
                        mMapEditorRef.SetMapData(std::unique_ptr<MapData>(rawMapData));
                    }
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {

                }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {

                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
private:
    MapEditor& mMapEditorRef;
};


// ======================================
MapEditor::MapEditor()
    : Shade::EditorBase("Map Editor")
{

}

// ======================================
void MapEditor::OnEnter() 
{
    Shade::ImGuiService* imguiService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ImGuiService>();
    imguiService->RegisterWindow(std::make_unique<MapEditorWindow>(*this));
}

// ======================================
void MapEditor::OnExit() 
{

}

// ======================================
void MapEditor::Update(float deltaSeconds) 
{
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    // TODO: y position update should come from a better place
    camera->SetCameraPosition(500.f, 360.f);
}

// ======================================
void MapEditor::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue) 
{
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();

    // Render the background of the current map
    if (mMapData)
    {
        for (const BackgroundElement& background : mMapData->GetBackgrounds())
        {
            Shade::ResourceHandle textureHandle = resourceManager->LoadResource<Shade::Texture>(background.mTexturePath);
            Shade::Texture* texture = resourceManager->GetResource<Shade::Texture>(textureHandle);
            const float drawX = Shade::RenderUtil::GetXForRenderAnchor(ParallaxUtil::GetParallaxPos(0.f, background.mParallax, camera), texture->GetWidth(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            const float drawY = Shade::RenderUtil::GetYForRenderAnchor(0.f, texture->GetHeight(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX, drawY, texture->GetWidth(), texture->GetHeight(), textureHandle, static_cast<int>(RenderLayer::BACKGROUND)));
        }
    }
}

// ======================================
bool MapEditor::HandleEvent(const Shade::InputEvent& event) 
{
    return false;
}


// ======================================
void MapEditor::SetMapData(std::unique_ptr<MapData> mapData)
{
    mMapData = std::move(mapData);
}
