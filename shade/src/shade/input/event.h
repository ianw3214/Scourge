#pragma once

#include "shade/input/keys.h"

namespace Shade {

    enum class InputEventType : uint8_t {
        KEY,
        MOUSE,
        CONTROLLER_BUTTON,
        CONTROLLER_AXIS
    };

    enum class KeyEventType : uint8_t {
        PRESS,
        RELEASE
    };

    enum class ButtonEventType : uint8_t {
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

        // CONTROLLER BUTTON EVENT INFO
        ButtonEventType mControllerButtonEvent;
        ControllerButton mControllerButton;

        // CONTROLLER AXIS EVENT INFO
        ControllerAxis mControllerAxis;
        int32_t mControllerValue;

        // Constructors for convenience
        static InputEvent CreateKeyPress(KeyCode key);
        static InputEvent CreateKeyRelease(KeyCode key);
        static InputEvent CreateButtonPress(ControllerButton button);
        static InputEvent CreateButtonRelease(ControllerButton button);
        static InputEvent CreateAxisEvent(ControllerAxis axis, int32_t value);
    };

}