#pragma once

#include "shade/common/collision.h"
#include "shade/common/vec.h"
#include "shade/file/keyValueFile.h"

#include <vector>

class MapLayout {
public:
    static MapLayout LoadFromKeyValueHandle(Shade::KeyValueHandle handle);
public:
    MapLayout() = default;
    MapLayout(const std::vector<Shade::Box>&& playZones);

    const std::vector<Shade::Box>& GetPlayZones() const;
private:
    // Gameplay related layout
    std::vector<Shade::Box> mPlayZones;
    Shade::Vec2 mPlayerStart;
};