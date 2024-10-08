#include "mapEditor.h"

#include <string>
#include <cassert>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "shade/file/fileSystem.h"
#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/drawCircle.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/graphics/command/drawRectangle.h"
#include "shade/graphics/command/setColourMultiplier.h"
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

// ======================================
namespace MapEditorConstants {
    // TODO: Specify where to find root "assets" folder and use relative paths to find files
    const std::string TempTargetFilePath = "assets/breach/maps/target.kv";
}

// ======================================
namespace MapEditorTextures {
    const std::string KnobTexture = "assets/editor/knob.png";
    const std::string UpArrowTexture = "assets/editor/up_arrow.png";
    const std::string RightArrowTexture = "assets/editor/right_arrow.png";
}

// ======================================
void PositionSliderWidget::Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{
    if (mShow)
    {
        const float drawX = mX - 20.f;
        const float drawY = mY - 20.f;
        Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
        Shade::ResourceHandle upArrowTextureHandle = resourceManager->LoadResource<Shade::Texture>(MapEditorTextures::UpArrowTexture);
        Shade::Texture* upArrowTexture = resourceManager->GetResource<Shade::Texture>(upArrowTextureHandle);
        Shade::ResourceHandle rightArrowTextureHandle = resourceManager->LoadResource<Shade::Texture>(MapEditorTextures::RightArrowTexture);
        Shade::Texture* rightArrowTexture = resourceManager->GetResource<Shade::Texture>(rightArrowTextureHandle);
        Shade::ResourceHandle textureHandle = resourceManager->LoadResource<Shade::Texture>(MapEditorTextures::KnobTexture);
        Shade::Texture* texture = resourceManager->GetResource<Shade::Texture>(textureHandle);
        if (mMouseOverWidgetPart == WidgetPart::VERTICAL)
        {
            commandQueue.emplace_back(std::make_unique<Shade::SetColourMultiplierCommand>(Shade::Colour{ 2.f, 2.f, 2.f }));
        }
        commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX, drawY + 20.f, static_cast<float>(upArrowTexture->GetWidth()), static_cast<float>(upArrowTexture->GetHeight()), upArrowTextureHandle, static_cast<int>(RenderLayer::UI)));
        if (mMouseOverWidgetPart == WidgetPart::VERTICAL)
        {
            commandQueue.emplace_back(std::make_unique<Shade::ResetColourMultiplierCommand>());
        }
        if (mMouseOverWidgetPart == WidgetPart::HORIZONTAL)
        {
            commandQueue.emplace_back(std::make_unique<Shade::SetColourMultiplierCommand>(Shade::Colour{ 2.f, 2.f, 2.f }));
        }
        commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX + 20.f, drawY, static_cast<float>(rightArrowTexture->GetWidth()), static_cast<float>(rightArrowTexture->GetHeight()), rightArrowTextureHandle, static_cast<int>(RenderLayer::UI)));
        if (mMouseOverWidgetPart == WidgetPart::HORIZONTAL)
        {
            commandQueue.emplace_back(std::make_unique<Shade::ResetColourMultiplierCommand>());
        }
        commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX, drawY, static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()), textureHandle, static_cast<int>(RenderLayer::UI)));
    }
}

// ======================================
bool PositionSliderWidget::HandleEvent(const Shade::InputEvent& event)
{
    if (event.mType == Shade::InputEventType::MOUSE)
    {
        if (event.mMouseEvent == Shade::MouseEventType::PRESS)
        {
            UpdateMouseOverWidgetPart(event.mMouseX, event.mMouseY);
            if (mMouseOverWidgetPart == WidgetPart::VERTICAL)
            {
                mDragging = true;
                mDragDirection = DragDirection::VERTICAL;
                return true;
            }
            if (mMouseOverWidgetPart == WidgetPart::HORIZONTAL)
            {
                mDragging = true;
                mDragDirection = DragDirection::HORIZONTAL;
                return true;
            }
        }
        if (event.mMouseEvent == Shade::MouseEventType::RELEASE && mDragging)
        {
            UpdateMouseOverWidgetPart(event.mMouseX, event.mMouseY);
            mDragging = false;
            return true;
        }
        if (event.mMouseEvent == Shade::MouseEventType::MOTION)
        {
            if (mDragging)
            {
                if (mDragDirection == DragDirection::HORIZONTAL)
                {
                    mUpdateHorizontalCallback(event.mRelativeMouseX);
                }
                if (mDragDirection == DragDirection::VERTICAL)
                {
                    mUpdateVerticalCallback(event.mRelativeMouseY);
                }
                return true;
            }
            else
            {
                UpdateMouseOverWidgetPart(event.mMouseX, event.mMouseY);
            }
        }
    }
    return false;
}

// ======================================
void PositionSliderWidget::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
}

// ======================================
void PositionSliderWidget::ShowWidget(std::function<void(float)> xUpdate, std::function<void(float)> yUpdate)
{
    mShow = true;
    mUpdateHorizontalCallback = xUpdate;
    mUpdateVerticalCallback = yUpdate;
}

// ======================================
void PositionSliderWidget::HideWidget()
{
    mShow = false;
}

// ======================================
//  - The passed in mouseX and mouseY should be raw screen coordinates
void PositionSliderWidget::UpdateMouseOverWidgetPart(float mouseX, float mouseY)
{
    const float drawX = mX - 20.f;
    const float drawY = mY - 20.f;

    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    Shade::Box verticalBox{ Shade::Vec2{ drawX, drawY + 50.f }, 40.f, 50.f };
    Shade::Box horizontalBox{ Shade::Vec2{ drawX + 50.f, drawY }, 50.f, 40.f };
    Shade::Vec2 mousePos = camera->ScreenToWorld(Shade::Vec2{ mouseX, mouseY });
    if (Shade::PointInBox(mousePos, verticalBox))
    {
        mMouseOverWidgetPart = WidgetPart::VERTICAL;
    }
    else if (Shade::PointInBox(mousePos, horizontalBox))
    {
        mMouseOverWidgetPart = WidgetPart::HORIZONTAL;
    }
    else
    {
        mMouseOverWidgetPart = WidgetPart::NONE;
    }
}

// ======================================
class EditorMapData : public MapData {
public:
    EditorMapData() : MapData("New map", {}, MapLayout{}) {}
};

// ======================================
class MapEditorWindow : public Shade::ImGuiWindow {
public:
    MapEditorWindow(MapEditor& mapEditorRef) : ImGuiWindow("Map editor window"), mMapEditorRef(mapEditorRef) {}
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
                        logger->LogError(std::string("Failed to create new map"));
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
                    mMapEditorRef.SaveFile();
                }
                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {
                    // TODO: Implement
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
                        backgrounds.emplace_back(BackgroundElement{ "new background", "assets/shade/default.png", 1.f});
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
                        // TODO: Can do dynamic position adding based on mouse cursor/camera or something
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

                std::vector<MapTransitionZone>& transitionZones = layout.GetMapTransitionsMutable();
                if (ImGui::TreeNode("Transition zones"))
                {
                    if (ImGui::Button("Add Transition Zone"))
                    {
                        transitionZones.emplace_back();
                    }
                    // Render transition zones in reverse order for consistency w/ backgrounds
                    char buf[32];
                    for (int n = transitionZones.size() - 1; n >= 0; n--)
                    {
                        sprintf(buf, "Transition %d", n);
                        if (ImGui::Selectable(buf, mMapEditorRef.IsSelected(SelectedType::TRANSITION_ZONE, n)))
                        {
                            mMapEditorRef.SelectTransitionZone(n);
                        }
                    }                                                                                                                                        
                    ImGui::TreePop();
                }

                // TODO: This is very basic interaction to use player start
                //  - Should hook this up w/ the slider widget to get things easier to work with
                if (ImGui::TreeNode("Player start"))
                {
                    ImGui::DragFloat("x", &layout.GetPlayerStartMutable().x, 1.f, -1000.f, 1000.f);
                    ImGui::DragFloat("y", &layout.GetPlayerStartMutable().y, 1.f, -1000.f, 1000.f);
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Camera"))
                {
                    ImGui::DragFloat("min x", &layout.GetCameraMinXMutable(), 1.f);
                    ImGui::DragFloat("max x", &layout.GetCameraMaxXMutable(), 1.f);
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
                        if (ImGui::Button("Duplicate"))
                        {
                            mMapEditorRef.DuplicateSelected();
                        }
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
                        ImGui::InputFloat("x", &playZone.mPosition.x, 1.f, 10.f);
                        ImGui::InputFloat("y", &playZone.mPosition.y, 1.f, 10.f);
                        ImGui::InputFloat("Width", &playZone.mWidth, 1.f, 10.f);
                        ImGui::InputFloat("Height", &playZone.mHeight, 1.f, 10.f);
                        if (ImGui::Button("Delete play zone"))
                        {
                            mMapEditorRef.DeleteSelected();
                        }
                        ImGui::TreePop();
                    }
                }

                if (mMapEditorRef.HasSelected(SelectedType::TRANSITION_ZONE))
                {
                    MapTransitionZone& transitionZone = transitionZones[mMapEditorRef.GetSelectedIndex()];
                    ImGui::SetNextItemOpen(true);
                    if (ImGui::TreeNode("Selected transition zone"))
                    {
                        ImGui::InputText("Transition", &(transitionZone.mMapTransition));
                        ImGui::InputFloat("x", &transitionZone.mZoneDefinition.mPosition.x, 1.f, 10.f);
                        ImGui::InputFloat("y", &transitionZone.mZoneDefinition.mPosition.y, 1.f, 10.f);
                        ImGui::InputFloat("Width", &transitionZone.mZoneDefinition.mWidth, 1.f, 10.f);
                        ImGui::InputFloat("Height", &transitionZone.mZoneDefinition.mHeight, 1.f, 10.f);
                        if (ImGui::Button("Delete transition zone"))
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
    imguiService->TryRegisterWindow(std::make_unique<MapEditorWindow>(*this));

    // TODO: Initialize camera position to some different (better) value
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    // TODO: y position update should come from a better place
    camera->SetCameraPosition(500.f, 360.f);
}

// ======================================
void MapEditor::OnExit() 
{
    // TODO: The string used here might want to use a shared const to avoid deleting the wrong window
    Shade::ImGuiService* imguiService = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ImGuiService>();
    imguiService->DeleteWindow("Map editor window");
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
            const float drawX = Shade::RenderUtil::GetXForRenderAnchor(ParallaxUtil::GetParallaxPos(background.mWorldX, background.mParallax, camera), texture->GetWidth(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            const float drawY = Shade::RenderUtil::GetYForRenderAnchor(background.mWorldY, texture->GetHeight(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            commandQueue.emplace_back(std::make_unique<Shade::DrawTextureCommand>(drawX, drawY, static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()), textureHandle, static_cast<int>(RenderLayer::BACKGROUND), true));
        }
        // Render map layout
        const MapLayout& layout = mMapData->GetLayout();
        for (const Shade::Box& playZone : layout.GetPlayZones())
        {
            commandQueue.emplace_back(std::make_unique<Shade::DrawRectangleCommand>(playZone, Shade::Colour{ 0.3f, 1.0f, 0.5f }, false));
        }
        for (const MapTransitionZone& transitionZone : layout.GetMapTransitions())
        {
            commandQueue.emplace_back(std::make_unique<Shade::DrawRectangleCommand>(transitionZone.mZoneDefinition, Shade::Colour{ 0.8f, 0.7f, 0.3f }, false));
        }
        commandQueue.emplace_back(std::make_unique<Shade::DrawCircleCommand>(layout.GetPlayerStart(), 12.f, Shade::Colour{ 0.8f, 0.6f, 0.4f}, false));
        // TODO: hard coded y position is not a vibe
        commandQueue.emplace_back(std::make_unique<Shade::DrawCircleCommand>(layout.GetCameraMinX(), 360.f, 8.f, Shade::Colour{ 0.6f, 0.8f, 0.4f}, false));
        commandQueue.emplace_back(std::make_unique<Shade::DrawCircleCommand>(layout.GetCameraMaxX(), 360.f, 8.f, Shade::Colour{ 0.6f, 0.8f, 0.4f}, false));

        // Render widgets on top of map elements
        // TODO: Play zones might be able to be rendered in the same way
        //  - Consider: Can the widget just automatically be rendered based on an offset w/o specifically checking the selected element?
        const std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgrounds();
        if (mSelectedType == SelectedType::BACKGROUND && mSelectedIndex >= 0 && mSelectedIndex < backgrounds.size())
        {
            const BackgroundElement& background = backgrounds[mSelectedIndex];
            const float widgetX = ParallaxUtil::GetParallaxPos(background.mWorldX, background.mParallax, camera);
            const float widgetY = background.mWorldY + 50.f;
            mSliderWidget.SetPosition(widgetX, widgetY);  

            // TODO: Either draw this earlier or cache the draw details to avoid a second search?
            Shade::ResourceHandle textureHandle = resourceManager->LoadResource<Shade::Texture>(background.mTexturePath);
            Shade::Texture* texture = resourceManager->GetResource<Shade::Texture>(textureHandle);
            const float drawX = Shade::RenderUtil::GetXForRenderAnchor(ParallaxUtil::GetParallaxPos(background.mWorldX, background.mParallax, camera), texture->GetWidth(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            const float drawY = Shade::RenderUtil::GetYForRenderAnchor(background.mWorldY, texture->GetHeight(), Shade::RenderAnchor::BOTTOM_MIDDLE);
            Shade::Box backgroundBox = Shade::Box{ Shade::Vec2{ drawX, drawY }, static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()) };
            commandQueue.emplace_back(std::make_unique<Shade::DrawRectangleCommand>(backgroundBox, Shade::Colour{ 0.9f, 0.6f, 0.1f}, false));
        }

        const std::vector<Shade::Box>& playZones = mMapData->GetLayout().GetPlayZones();
        if (mSelectedType == SelectedType::PLAY_ZONE && mSelectedIndex >= 0 && mSelectedIndex < playZones.size())
        {
            const Shade::Box& playZone = playZones[mSelectedIndex];
            const float drawX = playZone.mPosition.x;
            const float drawY = playZone.mPosition.y;
            mSliderWidget.SetPosition(drawX, drawY);  
        }

        const std::vector<MapTransitionZone>& transitionZones = mMapData->GetLayout().GetMapTransitions();
        if (mSelectedType == SelectedType::TRANSITION_ZONE && mSelectedIndex >= 0 && mSelectedIndex < playZones.size())
        {
            const MapTransitionZone& transitionZone = transitionZones[mSelectedIndex];
            const float drawX = transitionZone.mZoneDefinition.mPosition.x;
            const float drawY = transitionZone.mZoneDefinition.mPosition.y;
            mSliderWidget.SetPosition(drawX, drawY);  
        }

        mSliderWidget.Render(commandQueue);
    }
}

// ======================================
bool MapEditor::HandleEvent(const Shade::InputEvent& event) 
{
    if (mSliderWidget.HandleEvent(event))
    {
        return true;
    }

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
    // TODO: Generalize keyboard shortcuts to the base editor implementation
    if (event.mType == Shade::InputEventType::KEY)
    {
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_LCONTROL)
        {
            mControlPressed = event.mKeyEvent == Shade::KeyEventType::PRESS;
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
        if (event.mKeyCode == Shade::KeyCode::SHADE_KEY_D && mControlPressed)
        {
            if (event.mKeyEvent == Shade::KeyEventType::PRESS)
            {
                DuplicateSelected();
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
void MapEditor::SaveFile()
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    if (mMapData == nullptr)
    {
        logger->LogError("Tried saving map when no map is open...");
        return;
    }
    bool saveResult = mMapData->Save(MapEditorConstants::TempTargetFilePath);
    if (saveResult)
    {
        logger->LogInfo(std::string("Saved map to file '") + mMapData->GetName() + '\'');   
    }
    else
    {
        logger->LogError(std::string("Failed to open '") + MapEditorConstants::TempTargetFilePath + '\'');
    }
}

// ======================================
void MapEditor::SelectBackground(int index)
{
    assert(index >= 0 && index < mMapData->GetBackgrounds().size() && "Background index out of bounds");

    mSelectedType = SelectedType::BACKGROUND;
    mSelectedIndex = index;

    // This assumes the selected background is always in a good state
    //  - Might want more error checking, but is fine for now...
    BackgroundElement& background = mMapData->GetBackgroundsMutable()[mSelectedIndex];
    mSliderWidget.ShowWidget([&background](float xOffset){
        background.mWorldX += xOffset;
    }, [&background](float yOffset){
        background.mWorldY += yOffset;
    });
}

// ======================================
void MapEditor::SelectPlayZone(int index)
{
    assert(index >= 0 && index < mMapData->GetLayout().GetPlayZones().size() && "Play zone index out of bounds");

    mSelectedType = SelectedType::PLAY_ZONE;
    mSelectedIndex = index;

    // This assumes the selected play zone is always in a good state
    //  - Might want more error checking, but is fine for now...
    Shade::Box& box = mMapData->GetLayoutMutable().GetPlayZonesMutable()[mSelectedIndex];
    mSliderWidget.ShowWidget([&box](float xOffset){
        box.mPosition.x += xOffset;
    }, [&box](float yOffset){
        box.mPosition.y += yOffset;
    });
}

// ======================================
void MapEditor::SelectTransitionZone(int index)
{
    assert(index >= 0 && index < mMapData->GetLayout().GetMapTransitions().size() && "Map transition index out of bounds");

    mSelectedType = SelectedType::TRANSITION_ZONE;
    mSelectedIndex = index;

    // This assumes the selected map transition is always in a good state
    //  - Might want more error checking, but is fine for now...
    Shade::Box& box = mMapData->GetLayoutMutable().GetMapTransitionsMutable()[mSelectedIndex].mZoneDefinition;
    mSliderWidget.ShowWidget([&box](float xOffset){
        box.mPosition.x += xOffset;
    }, [&box](float yOffset){
        box.mPosition.y += yOffset;
    });
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
        if (mSelectedType == SelectedType::TRANSITION_ZONE)
        {
            const std::vector<MapTransitionZone>& transitions = mMapData->GetLayout().GetMapTransitions();
            return mSelectedIndex >= 0 && mSelectedIndex < transitions.size();
        }
    }
    return false;
}

// ======================================
void MapEditor::Unselect()
{
    mSelectedType = SelectedType::NONE; 
    mSelectedIndex = -1;

    mSliderWidget.HideWidget();
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
    if (mSelectedType == SelectedType::TRANSITION_ZONE)
    {
        std::vector<MapTransitionZone>& transitions = mMapData->GetLayoutMutable().GetMapTransitionsMutable();
        transitions.erase(transitions.begin() + mSelectedIndex);
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
        assert(false && "Ordering is irrelevant for play zones, do not implement...");
    }
    if (mSelectedType == SelectedType::TRANSITION_ZONE)
    {
        assert(false && "Ordering is irrelevant for transition zones, do not implement...");
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
        assert(false && "Ordering is irrelevant for play zones, do not implement...");
    }
    if (mSelectedType == SelectedType::TRANSITION_ZONE)
    {
        assert(false && "Ordering is irrelevant for transition zones, do not implement...");
    }
}

// ======================================
void MapEditor::DuplicateSelected()
{
    if (mSelectedType == SelectedType::BACKGROUND)
    {
        std::vector<BackgroundElement>& backgrounds = mMapData->GetBackgroundsMutable();
        backgrounds.emplace_back(backgrounds[mSelectedIndex]);

        backgrounds.back().mName.append("_copy");
        SelectBackground(backgrounds.size() - 1);
    }
    if (mSelectedType == SelectedType::PLAY_ZONE)
    {
        assert(false && "Duplication is not implemented for play zones, do not implement...");
    }
    if (mSelectedType == SelectedType::TRANSITION_ZONE)
    {
        assert(false && "Duplication is not implemented for transition zones, do not implement...");
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