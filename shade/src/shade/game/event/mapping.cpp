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
bool Shade::InputMapping::AddControllerButtonEventMapping(ControllerButton key, const std::string& event)
{
    if (mButtonEvents.find(key) == mButtonEvents.end())
    {
        mButtonEvents.emplace(key, event);
        return true;
    }
    return false;
}

// ======================================
bool Shade::InputMapping::AddAxisEventMapping(ControllerAxis key, const std::string& event)
{
    if (mAxisEvents.find(key) == mAxisEvents.end())
    {
        mAxisEvents.emplace(key, event);
        return true;
    }
    return false;
}

// ======================================
bool Shade::InputMapping::HasEventForKey(KeyCode key) const
{
    return mKeyEvents.find(key) != mKeyEvents.end();
}

// ======================================
bool Shade::InputMapping::HasEventForControllerButton(ControllerButton button) const
{
    return mButtonEvents.find(button) != mButtonEvents.end();
}

// ======================================
bool Shade::InputMapping::HasEventForAxis(ControllerAxis button) const
{
    return mAxisEvents.find(button) != mAxisEvents.end();
}

// ======================================
const std::string& Shade::InputMapping::GetKeyEvent(KeyCode key)
{
    if (mKeyEvents.find(key) == mKeyEvents.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        // TODO: Message formatting
        logService->LogError("Could not map input key {0} to gameplay event");
    }
    return mKeyEvents[key];
}

// ======================================
const std::string& Shade::InputMapping::GetControllerButtonEvent(ControllerButton key)
{
    if (mButtonEvents.find(key) == mButtonEvents.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        // TODO: Message formatting
        logService->LogError("Could not map input button {0} to gameplay event");
    }
    return mButtonEvents[key];
}

// ======================================
const std::string& Shade::InputMapping::GetControllerAxisEvent(ControllerAxis axis)
{
    if (mAxisEvents.find(axis) == mAxisEvents.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        // TODO: Message formatting
        logService->LogError("Could not map axis {0} to gameplay event");
    }
    return mAxisEvents[axis];
}

// ======================================
const std::unordered_map<Shade::KeyCode, std::string>& Shade::InputMapping::GetKeyEventMappings() const
{
    return mKeyEvents;
}

// ======================================
const std::unordered_map<Shade::ControllerButton, std::string>& Shade::InputMapping::GetControllerButtonEventMappings() const
{
    return mButtonEvents;
}

// ======================================
const std::unordered_map<Shade::ControllerAxis, std::string>& Shade::InputMapping::GetControllerAxisEventMappings() const
{
    return mAxisEvents;
}