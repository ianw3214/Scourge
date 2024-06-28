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
    MapLayout(std::vector<Shade::Box>&& playZones, std::vector<MapTransitionZone>&& transitions);

    const std::vector<Shade::Box>& GetPlayZones() const;
    const std::vector<MapTransitionZone>& GetMapTransitions() const;

private:
    // Gameplay related layout
    std::vector<Shade::Box> mPlayZones;
    std::vector<MapTransitionZone> mMapTransitions;
    Shade::Vec2 mPlayerStart;

#ifdef BUILD_BREACH_EDITOR
public:
    std::vector<Shade::Box>& GetPlayZonesMutable() { return mPlayZones; }
    std::vector<MapTransitionZone>& GetMapTransitionsMutable() { return mMapTransitions; }
#endif
};