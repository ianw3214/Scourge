#include "mapping.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/logging/logger.h"


// ======================================
bool Shade::InputMapping::AddKeyEventMapping(KeyCode Key, const std::string& Event)
{
    if (mKeyEvents.find(Key) == mKeyEvents.end())
    {
        mKeyEvents.emplace(Key, Event);
        return true;
    }
    return false;
}

// ======================================
const std::string& Shade::InputMapping::GetKeyEvent(KeyCode Key)
{
    if (mKeyEvents.find(Key) == mKeyEvents.end())
    {
        LogService* LogServiceInst = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        LogServiceInst->LogError("Could not map input key {0} to gameplay event");
    }
    return mKeyEvents[Key];
}

// ======================================
const std::unordered_map<Shade::KeyCode, std::string>& Shade::InputMapping::GetKeyEventMappings() const
{
    return mKeyEvents;
}