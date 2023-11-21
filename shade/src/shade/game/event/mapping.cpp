#include "mapping.h"

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
        // TODO: Maybe error here? not sure
    }
    return mKeyEvents[Key];
}

// ======================================
const std::unordered_map<Shade::KeyCode, std::string>& Shade::InputMapping::GetKeyEventMappings() const
{
    return mKeyEvents;
}