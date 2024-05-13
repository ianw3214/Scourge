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
MapLayout::MapLayout(const std::vector<Shade::Box>&& playZones)
    : mPlayZones(playZones)
{

}

// ======================================
const std::vector<Shade::Box>& MapLayout::GetPlayZones() const
{
    return mPlayZones;
}
