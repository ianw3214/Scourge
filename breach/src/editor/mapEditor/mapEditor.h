#pragma once

#include <memory>

#include "shade/editor/editorBase.h"

#include "shade/graphics/command/command.h"
#include "shade/input/event.h"

class MapData;

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

    void SelectBackground(int index);
    void SelectPlayZone(int index);

private:
    // Bypass resource system for storage since we want the editor to own this map data
    std::unique_ptr<MapData> mMapData = nullptr;

    // MapEditor state
    bool mPanning = false;
    float mPanStartX = 0.f;

    int mSelectedBackground = -1;
    int mSelectedPlayZone = -1;

    // Key states
    bool mControlPressed = false;
    bool mOpenReleased = true; // TODO: This migth be good to just handle in the engine
};
