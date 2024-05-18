#include "map.h"

#include "shade/file/fileSystem.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

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

    Shade::KeyValueHandle handle = file->GetContents();
    while(handle.IsValid())
    {
        if (handle.GetKey() == "name")
        {
            if (handle.IsString())
            {
                name = handle.GetString();
            }
            else
            {
                logger->LogWarning("Expected string for field 'name'");
            }
        }
        else if (handle.GetKey() == "layout")
        {
            if (handle.IsList())
            {
                layout = MapLayout::LoadFromKeyValueHandle(handle.GetListHead());
            }
            else
            {
                logger->LogWarning("Expected list for field 'layout'");
            }
        }
        else if (handle.GetKey() == "background")
        {
            if (handle.IsList())
            {
                Shade::KeyValueHandle backgroundListHandle = handle.GetListHead();
                while(backgroundListHandle.IsValid())
                {
                    BackgroundElement background;
                    // TODO: Perhaps the name only needs to be set in the editor, can omit for gameplay?
                    background.mName = backgroundListHandle.GetKey();
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
            else
            {
                logger->LogWarning("Expected list for field 'background'");
            }
        }
        else 
        {
            logger->LogWarning(std::string("Unexpected field while parsing map: ") + handle.GetKey());
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