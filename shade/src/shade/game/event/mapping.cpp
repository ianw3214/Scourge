#include "mapping.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/logging/logger.h"


// ======================================
bool Shade::InputMapping::AddKeyEventMapping(KeyCode key, const std::string& event)
{
    if (mKeyEvents.find(key) == mKeyEvents.end())
    {
        mKeyEvents.emplace(key, event);
        return true;
    }
    return false;
}

// ======================================
const std::string& Shade::InputMapping::GetKeyEvent(KeyCode key)
{
    if (mKeyEvents.find(key) == mKeyEvents.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        logService->LogError("Could not map input key {0} to gameplay event");
    }
    return mKeyEvents[key];
}

// ======================================
const std::unordered_map<Shade::KeyCode, std::string>& Shade::InputMapping::GetKeyEventMappings() const
{
    return mKeyEvents;
}