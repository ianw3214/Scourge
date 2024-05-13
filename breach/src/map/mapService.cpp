#include "mapService.h"

#include "shade/game/entity/component/spriteComponent.h"
#include "shade/graphics/common.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

#include "map/map.h"

#include "components/parallaxComponent.h"

// ======================================
MapService::MapService()
    : Shade::Service("MapService")
{

}

// ======================================
MapService::~MapService() = default;

// ======================================
std::vector<std::unique_ptr<Shade::Entity>> MapService::LoadMap(const std::string& path, Shade::GameWorldModule& gameWorld)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
    Shade::ResourceHandle mapDataHandle = resourceManager->LoadResource<MapData>(path);
    
    MapData* mapData = resourceManager->GetResource<MapData>(mapDataHandle);
    if (mapData == nullptr) 
    {
        logger->LogWarning(std::string("Failed to load map from path: ") + path);
        return {};
    }
    else
    {
        logger->LogInfo(std::string("Map loaded successfully: ") + mapData->GetName());
    }

    // TODO: Consider if the background should even be implemented as entities
    //  - There's usually not much interaction outside of just displaying an image w/ parallax
    std::vector<std::unique_ptr<Shade::Entity>> entities;
    for (const BackgroundElement& background : mapData->GetBackgrounds())
    {
        std::unique_ptr<Shade::Entity> backgroundEntity = std::make_unique<Shade::Entity>(gameWorld, gameWorld);
        // TODO: Figure out what to do about the renderlayer...
        backgroundEntity->AddComponent(std::make_unique<Shade::SpriteComponent>(background.mTexturePath, static_cast<int>(-5 /*RenderLayer::BACKGROUND*/), Shade::RenderAnchor::BOTTOM_MIDDLE));
        backgroundEntity->AddComponent(std::make_unique<HorizontalParallaxComponent>(background.mParallax));
        entities.emplace_back(std::move(backgroundEntity));
    }

    SetMapLayout(mapData->GetLayout());

    return entities;
}

// ======================================
const MapLayout& MapService::GetLayout() const
{
    return mCurrentLayout;
}

// ======================================
void MapService::SetMapLayout(MapLayout layout)
{
    mCurrentLayout = layout;
}