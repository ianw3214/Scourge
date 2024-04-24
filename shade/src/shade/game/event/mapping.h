#pragma once

#include <unordered_map>
#include <string>

#include "shade/input/keys.h"

namespace Shade {

    // `InputMapping` converts engine input events into engine gameplay events
    class InputMapping {
    public:
        bool AddKeyEventMapping(KeyCode key, const std::string& event);
        bool AddControllerButtonEventMapping(ControllerButton button, const std::string& event);
        bool AddAxisEventMapping(ControllerAxis axis, const std::string& event);

        bool HasEventForKey(KeyCode key) const;
        bool HasEventForControllerButton(ControllerButton button) const;
        bool HasEventForAxis(ControllerAxis axis) const;
        const std::string& GetKeyEvent(KeyCode key);
        const std::string& GetControllerButtonEvent(ControllerButton button);
        const std::string& GetControllerAxisEvent(ControllerAxis axis);

        const std::unordered_map<KeyCode, std::string>& GetKeyEventMappings() const;
        const std::unordered_map<ControllerButton, std::string>& GetControllerButtonEventMappings() const;
        const std::unordered_map<ControllerAxis, std::string>& GetControllerAxisEventMappings() const;
    private:
        // Consider changing events from strings to some other representation
        //  - Hashed string might be a convenient option to switch to
        std::unordered_map<KeyCode, std::string> mKeyEvents;

        // TODO: Consider if controller mappings should be considered as seperate from key mappings
        std::unordered_map<ControllerButton, std::string> mButtonEvents;
        std::unordered_map<ControllerAxis, std::string> mAxisEvents;
    };

}