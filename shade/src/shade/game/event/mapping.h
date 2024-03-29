#pragma once

#include <unordered_map>
#include <string>

#include "shade/input/keys.h"

namespace Shade {

    // `InputMapping` converts engine input events into engine gameplay events
    class InputMapping {
    public:
        bool AddKeyEventMapping(KeyCode key, const std::string& event);

        const std::string& GetKeyEvent(KeyCode key);

        const std::unordered_map<KeyCode, std::string>& GetKeyEventMappings() const;
    private:
        // Consider changing events from strings to some other representation
        //  - Hashed string might be a convenient option to switch to
        std::unordered_map<KeyCode, std::string> mKeyEvents;
    };

}