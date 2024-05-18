#include "mapEditor.h"

#include <string>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "shade/file/fileSystem.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/graphics/command/drawRectangle.h"
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

namespace MapEditorTextures {
    const std::string KnobTexture = "assets/editor/knob.png";
    const std::string UpArrowTexture = "assets/editor/up_arrow.png";
    const std::string RightArrowTexture = "assets/editor/right_arrow.png";
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

            std::unique_ptr<MapData>& mapData = mMapEditorRef.GetMapData();
            if (mapData != nullptr)
            {
                std::vector<BackgroundElement>& backgrounds = mapData->GetBackgroundsMutable();
                if (ImGui::TreeNode("Map backgrounds"))
                {
                    for (int n = 0; n < backgrounds.size(); n++)
                    {
                        if (ImGui::Selectable(backgrounds[n].mName.c_str(), mSelectedBackground == n))
                        {
                            mSelectedBackground = n;
                            mMapEditorRef.SelectBackground(n);
                        }
                    }
                    ImGui::TreePop();
                }

                if (mSelectedBackground >= 0 && mSelectedBackground < backgrounds.size())
                {
                    BackgroundElement& background = backgrounds[mSelectedBackground];
                    ImGui::SeparatorText("Selected background");
                    ImGui::InputText("Name", &(background.mName));
                    // TODO: This might be able to be implemented as a selectable list of existing paths
                    //  - Will require some more design work to not overload the options w/ texture options
                    //  - Is probably good enough to keep it working this way for a while with good error handling
                    ImGui::InputText("Texture path", &(background.mTexturePath));
                    ImGui::DragFloat("Parallax", &(background.mParallax), 0.1f, 0.f, 2.f);
                }
            }
        }

        ImGui::End();
    }
private:
    MapEditor& mMapEditorRef;

    // UI State
    int mSelectedBackground = -1;
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

    // TODO: Initialize camera position to some different (better) value
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    // TODO: y position update should come from a better place
    camera->SetCameraPosition(500.f, 360.f);
}

// ======================================
void MapEditor::OnExit() 
{

}

// ======================================
void MapEditor::Update(float deltaSeconds) 
{
    
}

// ======================================
void MapEditor::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue) 
{
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();

    // Render the background of the current map
    if (mMapData)
    {
        // Render map visuals
        for (const BackgroundElement& background : mMapData->GetBackgrounds())
        {
            Shade::ResourceHandle textureHandle = resourceManager->LoadResource<Shade::Texture>(background.mTexturePath);
            Shade::Texture* texture = resourceManager->GetResource<Shade::Texture>(textureHandle);
            const float drawX = Shade::RenderUtil::GetXForRenderAnchor(ParallaxUtil::GetParallaxPos(0.f, background.mParallax, camera), texture->GetWidth(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            const float drawY = Shade::RenderUtil::GetYForRenderAnchor(0.f, texture->GetHeight(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX, drawY, static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()), textureHandle, static_cast<int>(RenderLayer::BACKGROUND)));
        }
        // Render map layout
        const MapLayout& layout = mMapData->GetLayout();
        for (const Shade::Box& playZone : layout.GetPlayZones())
        {
            commandQueue.emplace_back(std::make_unique<Shade::DrawRectangleCommand>(playZone, Shade::Colour{ 0.3f, 1.0f, 0.5f }, false));
        }

        // Render widgets on top of map elements
        const std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgrounds();
        if (mSelectedBackground >= 0 && mSelectedBackground < backgrounds.size())
        {
            const BackgroundElement& background = backgrounds[mSelectedBackground];
            Shade::ResourceHandle upArrowTextureHandle = resourceManager->LoadResource<Shade::Texture>(MapEditorTextures::UpArrowTexture);
            Shade::Texture* upArrowTexture = resourceManager->GetResource<Shade::Texture>(upArrowTextureHandle);
            Shade::ResourceHandle rightArrowTextureHandle = resourceManager->LoadResource<Shade::Texture>(MapEditorTextures::RightArrowTexture);
            Shade::Texture* rightArrowTexture = resourceManager->GetResource<Shade::Texture>(rightArrowTextureHandle);
            Shade::ResourceHandle textureHandle = resourceManager->LoadResource<Shade::Texture>(MapEditorTextures::KnobTexture);
            Shade::Texture* texture = resourceManager->GetResource<Shade::Texture>(textureHandle);
            const float drawX = ParallaxUtil::GetParallaxPos(0.f, background.mParallax, camera) - static_cast<float>(texture->GetWidth() / 2.f);
            const float drawY = 50.f - static_cast<float>(texture->GetHeight() / 2.f);
            commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX, drawY + 20.f, static_cast<float>(upArrowTexture->GetWidth()), static_cast<float>(upArrowTexture->GetHeight()), upArrowTextureHandle, static_cast<int>(RenderLayer::UI)));
            commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX + 20.f, drawY, static_cast<float>(rightArrowTexture->GetWidth()), static_cast<float>(rightArrowTexture->GetHeight()), rightArrowTextureHandle, static_cast<int>(RenderLayer::UI)));
            commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX, drawY, static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()), textureHandle, static_cast<int>(RenderLayer::UI)));

            // TODO: Render outline of selected background as well
        }
    }
}

// ======================================
bool MapEditor::HandleEvent(const Shade::InputEvent& event) 
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    if (event.mType == Shade::InputEventType::MOUSE && event.mMouseEvent == Shade::MouseEventType::PRESS)
    {
        mPanning = true;
        return true;
    }
    if (event.mType == Shade::InputEventType::MOUSE && event.mMouseEvent == Shade::MouseEventType::RELEASE)
    {
        mPanning = false;
        return true;
    }
    if (mPanning)
    {
        if (event.mType == Shade::InputEventType::MOUSE && event.mMouseEvent == Shade::MouseEventType::MOTION)
        {
            Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
            const Shade::CameraInfo& camInfo = camera->GetCameraInfo();
            camera->SetCameraPosition(camInfo.x - event.mRelativeMouseX, camInfo.y);
            // Let mouse motion event fall through to other layers as well (maybe? idk)
            return false;
        }
    }
    return false;
}


// ======================================
void MapEditor::SetMapData(std::unique_ptr<MapData> mapData)
{
    mMapData = std::move(mapData);
}

// ======================================
std::unique_ptr<MapData>& MapEditor::GetMapData()
{
    return mMapData;
}

// ======================================
void MapEditor::SelectBackground(int index)
{
    mSelectedBackground = index;
}
