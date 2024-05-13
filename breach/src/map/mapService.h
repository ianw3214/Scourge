#pragma once

#include "shade/game/entity/entity.h"
#include "shade/game/world.h"
#include "shade/instance/service/service.h"

#include <memory>
#include <vector>

#include "map/layout.h"

// ======================================
class MapService : public Shade::Service {
public:
    MapService();
    ~MapService();

    // TODO: Make this somehow not have to pass in the entire game world to create the entities
    //  - Perhaps an entity factory can be passed in for creating entities
    //  - Alternatively, register entity factory as a service and locate the service to create new entities
    std::vector<std::unique_ptr<Shade::Entity>> LoadMap(const std::string& path, Shade::GameWorldModule& gameWorld);

    const MapLayout& GetLayout() const;

protected:
    void SetMapLayout(MapLayout layout);

private:
    MapLayout mCurrentLayout;
};
