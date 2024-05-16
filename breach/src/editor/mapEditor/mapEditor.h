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
private:
    // Bypass resource system for storage since we want the editor to own this map data
    std::unique_ptr<MapData> mMapData = nullptr;
};
