#pragma once

#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/instance/service/service.h"

#include <memory>
#include <vector>

#include "map/layout.h"

// ======================================
// - The map service 
class MapService : public Shade::Service {
public:
    MapService();
    ~MapService();

    bool LoadMap(const std::string& path);

    const MapLayout& GetLayout() const;

protected:
    void SetMapLayout(MapLayout layout);

private:
    MapLayout mCurrentLayout;
};
