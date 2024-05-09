#pragma once

#include "shade/common/collision.h"
#include "shade/common/vec.h"
#include "shade/resource/resource.h"

#include <vector>

class MapLayout : public Shade::Resource {
public:
    static Shade::Resource* Load(const std::string& path);

public:
    MapLayout(std::vector<Shade::Box>&& playZones);

    const std::vector<Shade::Box>& GetPlayZones() const;
private:
    std::vector<Shade::Box> mPlayZones;
    Shade::Vec2 mPlayerStart;
};