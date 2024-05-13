#include "map.h"

#include "shade/file/fileSystem.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

// TODO: Isolate out the RenderAnchor enum to not need to include this entire component
#include "shade/game/entity/component/spriteComponent.h"

#include "components/parallaxComponent.h"

// TODO: This is SUPERRRRRRRRRRRR hacky
//  - FIX ASAP, at the very least - find a good place to put this instead of "map.cpp" lol
const Shade::ResourceHandle Shade::ResourceHandle::Invalid = Shade::ResourceHandle(-1, -1);

// ======================================
Shade::Resource* MapData::Load(const std::string& path)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    Shade::FileSystem* fileSystem = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FileSystem>();
    std::unique_ptr<Shade::KeyValueFile> file = fileSystem->LoadKeyValueFile(path);

    if (file == nullptr)
    {
        logger->LogError(std::string("Could not load MapLayout, file was not loaded: ") + path);
        return nullptr;
    }

    std::string name;
    std::vector<BackgroundElement> backgrounds;
    MapLayout layout;

    // TODO: Error handling while parsing?
    Shade::KeyValueHandle handle = file->GetContents();
    while(handle.IsValid())
    {
        if (handle.GetKey() == "name")
        {
            name = handle.GetString();
        }
        if (handle.GetKey() == "layout")
        {
            layout = MapLayout::LoadFromKeyValueHandle(handle.GetListHead());
        }
        if (handle.GetKey() == "background")
        {
            Shade::KeyValueHandle backgroundListHandle = handle.GetListHead();
            while(backgroundListHandle.IsValid())
            {
                BackgroundElement background;
                Shade::KeyValueHandle backgroundHandle = backgroundListHandle.GetListHead();
                while (backgroundHandle.IsValid())
                {
                    
                    if (backgroundHandle.GetKey() == "path")
                    {
                        background.mTexturePath = backgroundHandle.GetString();
                    }
                    if (backgroundHandle.GetKey() == "parallax")
                    {
                        background.mParallax = backgroundHandle.GetFloat();
                    }
                    backgroundHandle.ToNext();
                }
                backgrounds.emplace_back(background);
                backgroundListHandle.ToNext();
            }
        }
        handle.ToNext();
    }

    return new MapData(name, std::move(backgrounds), std::move(layout));
}

// ======================================
MapData::MapData(const std::string& name, std::vector<BackgroundElement>&& backgrounds, const MapLayout&& layout)
    : mName(name)
    , mBackgrounds(backgrounds)
    , mLayout(layout)
{

}

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