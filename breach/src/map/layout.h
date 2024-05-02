#pragma once

#include "shade/common/collision.h"
#include "shade/common/vec.h"

#include <vector>

struct MapLayout {
    std::vector<Shade::Box> mPlayZones;
    Shade::Vec2 mPlayerStart;
};