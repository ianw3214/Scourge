#include "layout.h"

#include "shade/file/fileSystem.h"
#include "shade/instance/service/provider.h"

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
        handle.ToNext();
    }

    return new MapLayout(std::move(playZones));
}

// ======================================
MapLayout::MapLayout(std::vector<Shade::Box>&& playZones)
    : mPlayZones(playZones)
{

}

// ======================================
const std::vector<Shade::Box>& MapLayout::GetPlayZones() const
{
    return mPlayZones;
}
