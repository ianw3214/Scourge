#include "map.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
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

    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    const MapLayout* layout = GetLayout();
    if (layout == nullptr) 
    {
        // TODO: Should try to log what was attempted to be loaded as well
        logger->LogWarning("No map was loaded");
    }
    else
    {
        logger->LogInfo(std::string("Map loaded successfully: ") + layout->GetName());
    }
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