#include "source.h"

#include "shade/game/event/mapping.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
const Shade::BooleanGameplayEvent& Shade::GameplayEventSource::GetBooleanEvent(const std::string& eventName) const
{
    auto it = mBooleanEvents.find(eventName);
    if (it == mBooleanEvents.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        logService->LogError("Trying to access non-existent boolean event: " + eventName);
        return Shade::BooleanGameplayEvent{};   // <- This will basically crash the game anyways...
    }
    return it->second;
}

// ======================================
void Shade::GameplayEventSource::SetEventsFromMapping(const InputMapping& mapping)
{
    for (const auto& it : mapping.GetKeyEventMappings())
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
void Shade::GameplayEventSource::StartBooleanEvent(const std::string& eventName)
{
    auto it = mBooleanEvents.find(eventName);
    if (it == mBooleanEvents.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        logService->LogError("Trying to start boolean event that does not exist: " + eventName);
        return;
    }
    it->second.mTriggered = true;
    it->second.mHeld = true;
}

// ======================================
void Shade::GameplayEventSource::StopBooleanEvent(const std::string& eventName)
{
    auto it = mBooleanEvents.find(eventName);
    if (it == mBooleanEvents.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        logService->LogError("Trying to stop boolean event that does not exist: " + eventName);
        return;
    }
    it->second.mHeld = false;
    it->second.mReleased = true;
}
