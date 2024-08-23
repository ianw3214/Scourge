#pragma once

#include "shade/common/collision.h"
#include "shade/common/vec.h"
#include "shade/file/keyValueFile.h"

#include <vector>

// ======================================
struct MapTransitionZone {
    Shade::Box mZoneDefinition;
    std::string mMapTransition;
};

// ======================================
// The layout of the map contains relevant gameplay information about a map
class MapLayout {
public:
    static MapLayout LoadFromKeyValueHandle(Shade::KeyValueHandle handle);
    void SaveToKeyValueFile(Shade::KeyValueFile& file) const;
public:
    MapLayout() = default;
    MapLayout(std::vector<Shade::Box>&& playZones, std::vector<MapTransitionZone>&& transitions, Shade::Vec2 start, float min_x, float max_x);

    const std::vector<Shade::Box>& GetPlayZones() const;
    const std::vector<MapTransitionZone>& GetMapTransitions() const;
    const Shade::Vec2 GetPlayerStart() const;
    float GetCameraMinX() const;
    float GetCameraMaxX() const;

private:
    // Gameplay related layout
    std::vector<Shade::Box> mPlayZones;
    std::vector<MapTransitionZone> mMapTransitions;
    Shade::Vec2 mPlayerStart;

    // These values represent the bounds that the camera can travel to
    //  - The positions account for the size of the camera as well, not just the coordinate
    float mCameraMinX = -800.f;
    float mCameraMaxX = 800.f;

#ifdef BUILD_BREACH_EDITOR
public:
    std::vector<Shade::Box>& GetPlayZonesMutable() { return mPlayZones; }
    std::vector<MapTransitionZone>& GetMapTransitionsMutable() { return mMapTransitions; }
    Shade::Vec2& GetPlayerStartMutable() { return mPlayerStart; }
    float& GetCameraMinXMutable() { return mCameraMinX; }
    float& GetCameraMaxXMutable() { return mCameraMaxX; }
#endif
};