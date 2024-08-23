#include "layout.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
MapLayout MapLayout::LoadFromKeyValueHandle(Shade::KeyValueHandle handle)
{
    std::vector<Shade::Box> playZones;
    std::vector<MapTransitionZone> mapTransitions;
    Shade::Vec2 startPosition;
    float min_x = -800.f;
    float max_x = 800.f;

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
        if (handle.GetKey() == "transitions")
        {
            Shade::KeyValueHandle transitionListHandle = handle.GetListHead();
            while(transitionListHandle.IsValid())
            {
                MapTransitionZone transitionInfo;
                Shade::KeyValueHandle transitionHandle = transitionListHandle.GetListHead();
                while (transitionHandle.IsValid())
                {
                    if (transitionHandle.GetKey() == "transition")
                    {
                        transitionInfo.mMapTransition = transitionHandle.GetString();
                    }
                    if (transitionHandle.GetKey() == "x")
                    {
                        transitionInfo.mZoneDefinition.mPosition.x = transitionHandle.GetFloat();
                    }
                    if (transitionHandle.GetKey() == "y")
                    {
                        transitionInfo.mZoneDefinition.mPosition.y = transitionHandle.GetFloat();
                    }
                    if (transitionHandle.GetKey() == "w")
                    {
                        transitionInfo.mZoneDefinition.mWidth = transitionHandle.GetFloat();
                    }
                    if (transitionHandle.GetKey() == "h")
                    {
                        transitionInfo.mZoneDefinition.mHeight = transitionHandle.GetFloat();
                    }
                    transitionHandle.ToNext();
                }
                mapTransitions.emplace_back(transitionInfo);
                transitionListHandle.ToNext();
            }
        }
        if (handle.GetKey() == "start")
        {
            Shade::KeyValueHandle startHandle = handle.GetListHead();
            while(startHandle.IsValid())
            {
                if (startHandle.GetKey() == "x")
                {
                    startPosition.x = startHandle.GetFloat();
                }
                if (startHandle.GetKey() == "y")
                {
                    startPosition.y = startHandle.GetFloat();
                }
                startHandle.ToNext();
            }
        }
        if (handle.GetKey() == "camera")
        {
            Shade::KeyValueHandle cameraHandle = handle.GetListHead();
            while(cameraHandle.IsValid())
            {
                if (cameraHandle.GetKey() == "min_x")
                {
                    min_x = cameraHandle.GetFloat();
                }
                if (cameraHandle.GetKey() == "max_x")
                {
                    max_x = cameraHandle.GetFloat();
                }
                cameraHandle.ToNext();
            }
        }
        handle.ToNext();
    }
    return MapLayout(std::move(playZones), std::move(mapTransitions), startPosition, min_x, max_x);
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

    file.PushList("transitions");
    index = 0;
    for (const MapTransitionZone& transition : mMapTransitions)
    {
        file.PushList(std::string("transition") + std::to_string(index));
        file.AddStringEntry("transition", transition.mMapTransition);
        file.AddFloatEntry("x", transition.mZoneDefinition.mPosition.x);
        file.AddFloatEntry("y", transition.mZoneDefinition.mPosition.y);
        file.AddFloatEntry("w", transition.mZoneDefinition.mWidth);
        file.AddFloatEntry("h", transition.mZoneDefinition.mHeight);
        file.PopList();
        index++;
    }
    file.PopList();

    file.PushList("start");
    file.AddFloatEntry("x", mPlayerStart.x);
    file.AddFloatEntry("y", mPlayerStart.y);
    file.PopList();

    file.PushList("camera");
    file.AddFloatEntry("min_x", mCameraMinX);
    file.AddFloatEntry("max_x", mCameraMaxX);
    file.PopList();
}

// ======================================
MapLayout::MapLayout(std::vector<Shade::Box>&& playZones, std::vector<MapTransitionZone>&& transitions, Shade::Vec2 start, float min_x, float max_x)
    : mPlayZones(playZones)
    , mMapTransitions(transitions)
    , mPlayerStart(start)
    , mCameraMinX(min_x)
    , mCameraMaxX(max_x)
{

}

// ======================================
const std::vector<Shade::Box>& MapLayout::GetPlayZones() const
{
    return mPlayZones;
}

// ======================================
const std::vector<MapTransitionZone>& MapLayout::GetMapTransitions() const
{
    return mMapTransitions;
}

// ======================================
const Shade::Vec2 MapLayout::GetPlayerStart() const
{
    return mPlayerStart;
}

// ======================================
float MapLayout::GetCameraMinX() const
{
    return mCameraMinX;
}

// ======================================
float MapLayout::GetCameraMaxX() const
{
    return mCameraMaxX;
}
