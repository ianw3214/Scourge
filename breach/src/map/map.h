#pragma once

#include <memory>
#include <vector>
#include <string>

#include "shade/instance/service/service.h"
#include "shade/resource/handle.h"

#include "map/layout.h"

class MapService : public Shade::Service {
public:
    MapService();
    ~MapService();

    void SetLayoutResource(Shade::ResourceHandle handle);
    const MapLayout* GetLayout() const;

private:
    Shade::ResourceHandle mMapLayoutHandle = Shade::ResourceHandle::Invalid;
};
