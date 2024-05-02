#pragma once

#include <memory>
#include <vector>
#include <string>

#include "shade/instance/service/service.h"

#include "map/layout.h"

class MapService : public Shade::Service {
public:
    MapService();
    ~MapService();

    void SetLayout(MapLayout layout);
    const MapLayout& GetLayout() const;

private:
    MapLayout mCurrentLayout;
};
