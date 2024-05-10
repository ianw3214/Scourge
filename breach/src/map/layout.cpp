#include "layout.h"

#include "shade/file/fileSystem.h"
#include "shade/instance/service/provider.h"

// TODO: Isolate out the RenderAnchor enum to not need to include this entire component
#include "shade/game/entity/component/spriteComponent.h"

// TODO: Temp - remove
#include "shade/logging/logService.h"

#include "components/parallaxComponent.h"

// ======================================
Shade::Resource* MapLayout::Load(const std::string& path)
{
    Shade::FileSystem* fileSystem = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FileSystem>();
    std::unique_ptr<Shade::KeyValueFile> file = fileSystem->LoadKeyValueFile(path);

    if (file == nullptr)
    {
        // TODO: Error
        return nullptr;
    }

    std::vector<Shade::Box> playZones;
    std::vector<BackgroundElement> backgrounds;

    // TODO: Error handling while parsing?
    Shade::KeyValueHandle handle = file->GetContents();
    while(handle.IsValid())
    {
        if (handle.GetKey() == "zones")
        {
            Shade::KeyValueHandle zoneListHandle = handle.GetListHead();
            while(zoneListHandle.IsValid())
            {
                Shade::Box zoneInfo;
                Shade::KeyValueHandle zoneHandle = zoneListHandle.GetListHead();
                while (zoneHandle.IsValid())
                {
                    if (zoneHandle.GetKey() == "x")
                    {
                        zoneInfo.mPosition.x = zoneHandle.GetFloat();
                    }
                    if (zoneHandle.GetKey() == "y")
                    {
                        zoneInfo.mPosition.y = zoneHandle.GetFloat();
                    }
                    if (zoneHandle.GetKey() == "w")
                    {
                        zoneInfo.mWidth = zoneHandle.GetFloat();
                    }
                    if (zoneHandle.GetKey() == "h")
                    {
                        zoneInfo.mHeight = zoneHandle.GetFloat();
                    }
                    zoneHandle.ToNext();
                }
                playZones.emplace_back(zoneInfo);
                zoneListHandle.ToNext();
            }
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

    return new MapLayout(std::move(playZones), std::move(backgrounds));
}

// ======================================
MapLayout::MapLayout(std::vector<Shade::Box>&& playZones, std::vector<BackgroundElement>&& backgrounds)
    : mPlayZones(playZones)
    , mBackgrounds(backgrounds)
{

}

// ======================================
std::vector<std::unique_ptr<Shade::Entity>> MapLayout::CreateGameEntities(Shade::GameWorldModule& gameWorld)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();

    // TODO: Consider if the background should even be implemented as entities
    //  - There's usually not much interaction outside of just displaying an image w/ parallax
    std::vector<std::unique_ptr<Shade::Entity>> entities;
    for (const BackgroundElement& background : mBackgrounds)
    {
        std::unique_ptr<Shade::Entity> backgroundEntity = std::make_unique<Shade::Entity>(gameWorld, gameWorld);
        backgroundEntity->AddComponent(std::make_unique<Shade::SpriteComponent>(background.mTexturePath, static_cast<int>(-5 /*RenderLayer::BACKGROUND*/), Shade::RenderAnchor::BOTTOM_MIDDLE));
        backgroundEntity->AddComponent(std::make_unique<HorizontalParallaxComponent>(background.mParallax));
        entities.emplace_back(std::move(backgroundEntity));
    }
    return entities;
}

// ======================================
const std::vector<Shade::Box>& MapLayout::GetPlayZones() const
{
    return mPlayZones;
}
