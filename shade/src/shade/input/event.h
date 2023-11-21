#pragma once

#include "shade/input/keys.h"

namespace Shade {

    enum class InputEventType {
        KEY,
        MOUSE
    };

    enum class KeyEventType {
        PRESS,
        RELEASE
    };

    // mimics SDLs event struct, using own version so "backend" can be swapped out in the future
    struct InputEvent {
        InputEventType mType;

        // KEY EVENT INFO
        KeyEventType mKeyEvent;
        KeyCode mKeyCode;

        // MOUSE EVENT INFO
        // TODO: Working on this...

        // Constructors for convenience
        static InputEvent CreateKeyPress(KeyCode Key);
        static InputEvent CreateKeyRelease(KeyCode Key);
    };

}