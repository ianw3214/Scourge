#include "map.h"

#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"

// TODO: This is SUPERRRRRRRRRRRR hacky
//  - FIX ASAP, at the very least - find a good place to put this instead of "map.cpp" lol
const Shade::ResourceHandle Shade::ResourceHandle::Invalid = Shade::ResourceHandle(-1, -1);

// ======================================
MapService::MapService()
    : Shade::Service("MapService")
{

}

// ======================================
MapService::~MapService() = default;

// ======================================
void MapService::SetLayoutResource(Shade::ResourceHandle mapResource)
{
    mMapLayoutHandle = mapResource;
}

// ======================================
const MapLayout* MapService::GetLayout() const
{
    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
    return resourceManager->GetResource<MapLayout>(mMapLayoutHandle);
}

// ======================================
std::vector<std::unique_ptr<Shade::Entity>> MapService::CreateGameEntities(Shade::GameWorldModule& gameWorld)
{
    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
    MapLayout* layout = resourceManager->GetResource<MapLayout>(mMapLayoutHandle);

    return layout->CreateGameEntities(gameWorld);
}