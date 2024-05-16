#pragma once

#include <memory>

#include "shade/editor/editorBase.h"

class MapData;

class MapEditor : public Shade::EditorBase {
public:
    MapEditor();

    virtual void OnEnter() override;
    virtual void OnExit() override;

    void SetMapData(std::unique_ptr<MapData> mapData);
private:
    // Bypass resource system for storage since we want the editor to own this map data
    std::unique_ptr<MapData> mMapData = nullptr;
};
