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

    bool HasQueuedMap() const;
    void QueueMap(const std::string& path);
    bool LoadMap(const std::string& path);
    bool LoadQueuedMap();

    const MapLayout& GetLayout() const;

protected:
    void SetMapLayout(MapLayout layout);

private:
    MapLayout mCurrentLayout;
    std::string mQueuedMap;
};
