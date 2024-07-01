#include "mapService.h"

#include "definitions.h"

#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/factory.h"
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
bool MapService::LoadMap(const std::string& path)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    Shade::EntityFactory* entityFactory = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::EntityFactory>();
    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();

    Shade::ResourceHandle mapDataHandle = resourceManager->LoadResource<MapData>(path);
    MapData* mapData = resourceManager->GetResource<MapData>(mapDataHandle);
    if (mapData == nullptr) 
    {
        logger->LogWarning(std::string("Failed to load map from path: ") + path);
        return false;
    }
    else
    {
        logger->LogInfo(std::string("Map loaded from file: ") + mapData->GetName());
    }

    SetMapLayout(mapData->GetLayout());

    // TODO: Consider if the background should even be implemented as entities
    //  - There's usually not much interaction outside of just displaying an image w/ parallax
    for (const BackgroundElement& background : mapData->GetBackgrounds())
    {
        std::unique_ptr<Shade::Entity>& backgroundEntity = entityFactory->CreateAndRegisterNewEntity();
        backgroundEntity->AddComponent(std::make_unique<Shade::SpriteComponent>(background.mTexturePath, static_cast<int>(RenderLayer::BACKGROUND), Shade::RenderAnchor::BOTTOM_MIDDLE, true));
        backgroundEntity->AddComponent(std::make_unique<HorizontalParallaxComponent>(background.mParallax, background.mWorldX));
        backgroundEntity->SetPositionY(background.mWorldY);
    }    

    return true;
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