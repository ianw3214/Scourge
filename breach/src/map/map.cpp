#include "map.h"

// ======================================
MapService::MapService()
    : Shade::Service("MapService")
{

}

// ======================================
MapService::~MapService() = default;

// ======================================
void MapService::SetLayout(MapLayout layout)
{
    mCurrentLayout = layout;
}

// ======================================
const MapLayout& MapService::GetLayout() const
{
    return mCurrentLayout;
}