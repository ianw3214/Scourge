#pragma once

#include <memory>
#include <functional>

#include "shade/editor/editorBase.h"

#include "shade/graphics/command/command.h"
#include "shade/input/event.h"

class MapData;

enum class SelectedType {
    NONE,
    BACKGROUND,
    PLAY_ZONE,
    TRANSITION_ZONE,
};

// TODO: Consider if this should be shared w/ other widgets, maybe belongs in engine code
class PositionSliderWidget {
public:
    void Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue);
    bool HandleEvent(const Shade::InputEvent& event);

    void SetPosition(float x, float y);

    void ShowWidget(std::function<void(float)> xUpdate, std::function<void(float)> yUpdate);
    void HideWidget();

private:
    void UpdateMouseOverWidgetPart(float mouseX, float mouseY);

private:
    bool mShow = false;
    float mX = 0.f;
    float mY = 0.f;

    enum class WidgetPart : char {
        NONE,
        HORIZONTAL,
        VERTICAL,
        KNOB
    };
    WidgetPart mMouseOverWidgetPart = WidgetPart::NONE;

    enum class DragDirection : char {
        HORIZONTAL,
        VERTICAL
    };
    bool mDragging = false;
    DragDirection mDragDirection;

    // These update functions pass in the delta offsets as their parameters
    std::function<void(float)> mUpdateHorizontalCallback;
    std::function<void(float)> mUpdateVerticalCallback;
};

// TODO: More things might want to be shared between background elements and play zones to make refactoring easier
//  - e.g. Common base class
class MapEditor : public Shade::EditorBase {
public:
    MapEditor();

    virtual void OnEnter() override;
    virtual void OnExit() override;

    virtual void Update(float deltaSeconds) override;
    virtual void Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue) override;
    virtual bool HandleEvent(const Shade::InputEvent& event) override;

    void SetMapData(std::unique_ptr<MapData> mapData);
    std::unique_ptr<MapData>& GetMapData();
    void OpenFile();
    void SaveFile();

    void SelectBackground(int index);
    void SelectPlayZone(int index);
    void SelectTransitionZone(int index);
    void Unselect();
    void DeleteSelected();
    void MoveSelectedUp();
    void MoveSelectedDown();
    void DuplicateSelected();
    bool HasSelected(SelectedType type) const;
    bool IsSelected(SelectedType type, int index) const;
    int GetSelectedIndex() const;

private:
    // Bypass resource system for storage since we want the editor to own this map data
    std::unique_ptr<MapData> mMapData = nullptr;

    // MapEditor state
    bool mPanning = false;
    float mPanStartX = 0.f;

    SelectedType mSelectedType = SelectedType::NONE;
    int mSelectedIndex = -1;

    // Widgets
    PositionSliderWidget mSliderWidget;

    // Input states
    bool mControlPressed = false;
};
