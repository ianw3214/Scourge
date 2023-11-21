#pragma once

#include <string>
#include <unordered_map>

#include "shade/game/event/event.h"

namespace Shade {

    class InputMapping;

    class GameplayEventSource {
    public:
        const BooleanGameplayEvent& GetBooleanEvent(const std::string& EventName) const;

    protected:
        void SetEventsFromMapping(const InputMapping& Mapping);
        // Make sure this gets called before the actual handling of input events
        //  so that state being set from input events are retained and not cleared
        void UpdateEventStates();

        void StartBooleanEvent(const std::string& EventName);
        void StopBooleanEvent(const std::string& EventName);

    private:
        std::unordered_map<std::string, BooleanGameplayEvent> mBooleanEvents;
    };

}