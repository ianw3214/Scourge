#include "mapTransitionComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/logging/logger.h"

#include "map/mapService.h"

// ======================================
void MapTransitionComponent::Update(float deltaSeconds) 
{
    MapService* map = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
    const MapLayout& layout = map->GetLayout();
    for (const MapTransitionZone& transition : layout.GetMapTransitions())
    {
        if (Shade::PointInBox(mEntityRef->GetPosition(), transition.mZoneDefinition))
        {
            Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
            logger->LogInfo("Changing to map: " + transition.mMapTransition);

            MapService* mapService = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
            mapService->QueueMap(transition.mMapTransition);
        }
    }
}