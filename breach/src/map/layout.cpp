#include "layout.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
MapLayout MapLayout::LoadFromKeyValueHandle(Shade::KeyValueHandle handle)
{
    std::vector<Shade::Box> playZones;
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
    return MapLayout(std::move(playZones));
}

// ======================================
//  - Assumes the depth in the file is already setup properly
//  - Assumes the caller has already set up the list for this to be serialized into
void MapLayout::SaveToKeyValueFile(Shade::KeyValueFile& file) const
{
    file.PushList("zones");
    int index = 0;
    for (const Shade::Box& playZone : mPlayZones)
    {
        file.PushList(std::string("zone") + std::to_string(index));
        file.AddFloatEntry("x", playZone.mPosition.x);
        file.AddFloatEntry("y", playZone.mPosition.y);
        file.AddFloatEntry("w", playZone.mWidth);
        file.AddFloatEntry("h", playZone.mHeight);
        file.PopList();
        index++;
    }
    file.PopList();
}

// ======================================
MapLayout::MapLayout(const std::vector<Shade::Box>&& playZones)
    : mPlayZones(playZones)
{

}

// ======================================
const std::vector<Shade::Box>& MapLayout::GetPlayZones() const
{
    return mPlayZones;
}
