#include "source.h"

#include "shade/game/event/mapping.h"

// ======================================
const Shade::BooleanGameplayEvent& Shade::GameplayEventSource::GetBooleanEvent(const std::string& EventName) const
{
    auto it = mBooleanEvents.find(EventName);
    if (it == mBooleanEvents.end())
    {
        // TODO: Error...
    }
    return it->second;
}

// ======================================
void Shade::GameplayEventSource::SetEventsFromMapping(const InputMapping& Mapping)
{
    for (const auto& it : Mapping.GetKeyEventMappings())
    {
        mBooleanEvents.emplace(it.second, BooleanGameplayEvent{});
    }
}

// ======================================
void Shade::GameplayEventSource::UpdateEventStates()
{
    for (auto& it : mBooleanEvents)
    {
        it.second.mTriggered = false;
        it.second.mReleased = false;
    }
}

// ======================================
void Shade::GameplayEventSource::StartBooleanEvent(const std::string& EventName)
{
    auto it = mBooleanEvents.find(EventName);
    if (it == mBooleanEvents.end())
    {
        // TODO: Error...
        return;
    }
    it->second.mTriggered = true;
    it->second.mHeld = true;
}

// ======================================
void Shade::GameplayEventSource::StopBooleanEvent(const std::string& EventName)
{
    auto it = mBooleanEvents.find(EventName);
    if (it == mBooleanEvents.end())
    {
        // TODO: Error...
        return;
    }
    it->second.mHeld = false;
    it->second.mReleased = true;
}
