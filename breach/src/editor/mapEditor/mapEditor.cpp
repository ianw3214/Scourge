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
                    mMapEditorRef.OpenFile();
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
                    if (ImGui::Button("Add Background"))
                    {
                        backgrounds.emplace_back(BackgroundElement{ "new background", "assets/textures/default.png", 1.f});
                    }
                    // Show backgrounds in reverse order so furthest back backround shows up on the bottom of the screen
                    for (int n = backgrounds.size() - 1; n >= 0; n--)
                    {
                        if (ImGui::Selectable(backgrounds[n].mName.c_str(), mMapEditorRef.IsSelected(SelectedType::BACKGROUND, n)))
                        {
                            mMapEditorRef.SelectBackground(n);
                        }
                    }
                    ImGui::TreePop();
                }

                MapLayout& layout = mapData->GetLayoutMutable();
                std::vector<Shade::Box>& playZones = layout.GetPlayZonesMutable();
                if (ImGui::TreeNode("Play zones"))
                {
                    if (ImGui::Button("Add Play Zone"))
                    {
                        playZones.emplace_back(Shade::Vec2{0.f, 0.f}, 10.f, 10.f);
                    }
                    // Render play zones in reverse order for consistency w/ backgrounds
                    char buf[32];
                    for (int n = playZones.size() - 1; n >= 0; n--)
                    {
                        sprintf(buf, "Zone %d", n);
                        if (ImGui::Selectable(buf, mMapEditorRef.IsSelected(SelectedType::BACKGROUND, n)))
                        {
                            mMapEditorRef.SelectPlayZone(n);
                        }
                    }                                                                                                                                               
                    ImGui::TreePop();
                }

                if (mMapEditorRef.HasSelected(SelectedType::BACKGROUND))
                {
                    BackgroundElement& background = backgrounds[mMapEditorRef.GetSelectedIndex()];
                    ImGui::SetNextItemOpen(true);
                    if (ImGui::TreeNode("Selected background"))
                    {
                        ImGui::InputText("Name", &(background.mName));
                        // TODO: This might be able to be implemented as a selectable list of existing paths
                        //  - Will require some more design work to not overload the options w/ texture options
                        //  - Is probably good enough to keep it working this way for a while with good error handling
                        ImGui::InputText("Texture path", &(background.mTexturePath));
                        ImGui::DragFloat("Parallax", &(background.mParallax), 0.1f, 0.f, 2.f);
                        if (ImGui::Button("Delete background"))
                        {
                            mMapEditorRef.DeleteSelected();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Move up"))
                        {
                            mMapEditorRef.MoveSelectedUp();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Move down"))
                        {
                            mMapEditorRef.MoveSelectedDown();
                        }
                        ImGui::TreePop();
                    }
                }

                if (mMapEditorRef.HasSelected(SelectedType::PLAY_ZONE))
                {
                    Shade::Box& playZone = playZones[mMapEditorRef.GetSelectedIndex()];
                    ImGui::SetNextItemOpen(true);
                    if (ImGui::TreeNode("Selected play zone"))
                    {
                        char buf[32];
                        sprintf(buf, "Zone %d", mMapEditorRef.GetSelectedIndex());
                        ImGui::Text("Name", buf);
                        ImGui::InputFloat("Width", &playZone.mWidth, 1.f, 10.f);
                        ImGui::InputFloat("Height", &playZone.mHeight, 1.f, 10.f);
                        if (ImGui::Button("Delete play zone"))
                        {
                            mMapEditorRef.DeleteSelected();
                        }
                        ImGui::TreePop();
                    }
                }
            }
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
        // TODO: Play zones might be able to be rendered in the same way
        const std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgrounds();
        if (mSelectedType == SelectedType::BACKGROUND && mSelectedIndex >= 0 && mSelectedIndex < backgrounds.size())
        {
            const BackgroundElement& background = backgrounds[mSelectedIndex];
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
    if (event.mType == Shade::InputEventType::KEY)
    {
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_LCONTROL)
        {
            mControlPressed = event.mKeyEvent == Shade::KeyEventType::PRESS;
        }
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_O)
        {
            if (event.mKeyEvent == Shade::KeyEventType::PRESS && mOpenReleased)
            {
                mOpenReleased = false;
                OpenFile();
            }
            if (event.mKeyEvent == Shade::KeyEventType::RELEASE)
            {
                mOpenReleased = true;
            }
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
void MapEditor::OpenFile()
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    MapData* rawMapData = dynamic_cast<MapData*>(MapData::Load(MapEditorConstants::TempTargetFilePath));
    if (rawMapData == nullptr)
    {
        logger->LogError(std::string("Failed to open '") + MapEditorConstants::TempTargetFilePath + '\'');
    }
    else
    {
        logger->LogInfo(std::string("Opened '") + rawMapData->GetName() + '\'');   
        SetMapData(std::unique_ptr<MapData>(rawMapData));
    }
}

// ======================================
void MapEditor::SelectBackground(int index)
{
    // TODO: Check the index before storing? can be an assert
    mSelectedType = SelectedType::BACKGROUND;
    mSelectedIndex = index;
}

// ======================================
void MapEditor::SelectPlayZone(int index)
{
    // TODO: Check the index before storing? can be an assert
    mSelectedType = SelectedType::PLAY_ZONE;
    mSelectedIndex = index;
}

// ======================================
bool MapEditor::HasSelected(SelectedType type) const
{
    if (mSelectedType == type)
    {
        if (mSelectedType == SelectedType::BACKGROUND)
        {
            std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgroundsMutable();
            return mSelectedIndex >= 0 && mSelectedIndex < backgrounds.size();
        }
        if (mSelectedType == SelectedType::PLAY_ZONE)
        {
            const std::vector<Shade::Box>& playZones = mMapData->GetLayout().GetPlayZones();
            return mSelectedIndex >= 0 && mSelectedIndex < playZones.size();
        }
    }
    return false;
}

// ======================================
void MapEditor::Unselect()
{
    mSelectedType = SelectedType::NONE; 
    mSelectedIndex = -1;
}

// ======================================
void MapEditor::DeleteSelected()
{
    if (mSelectedType == SelectedType::BACKGROUND)
    {
        std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgroundsMutable();
        backgrounds.erase(backgrounds.begin() + mSelectedIndex);
    }
    if (mSelectedType == SelectedType::PLAY_ZONE)
    {
        std::vector<Shade::Box>& playZones = mMapData->GetLayoutMutable().GetPlayZonesMutable();
        playZones.erase(playZones.begin() + mSelectedIndex);
    }
    Unselect();
}

// ======================================
void MapEditor::MoveSelectedUp()
{
    if (mSelectedType == SelectedType::BACKGROUND)
    {
        std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgroundsMutable();
        if (mSelectedIndex + 1 < backgrounds.size())
        {
            std::swap(backgrounds[mSelectedIndex], backgrounds[mSelectedIndex + 1]);
            mSelectedIndex++;
        }
    }
    if (mSelectedType == SelectedType::PLAY_ZONE)
    {
        // TODO: Assert here
        //  - Order doesn't matter for play zones, this should not need to be implemented
    }
}

// ======================================
void MapEditor::MoveSelectedDown()
{
    if (mSelectedType == SelectedType::BACKGROUND)
    {
        if (mSelectedIndex -1 >= 0)
        {
            std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgroundsMutable();
            std::swap(backgrounds[mSelectedIndex], backgrounds[mSelectedIndex - 1]);
            mSelectedIndex--;
        }
    }
    if (mSelectedType == SelectedType::PLAY_ZONE)
    {
        // TODO: Assert here
        //  - Order doesn't matter for play zones, this should not need to be implemented
    }
}

// ======================================
bool MapEditor::IsSelected(SelectedType type, int index) const
{
    return mSelectedType == type && mSelectedIndex == index;
}

// ======================================
int MapEditor::GetSelectedIndex() const
{
    return mSelectedIndex;
}