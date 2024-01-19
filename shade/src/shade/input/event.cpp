#include "event.h"

// ======================================
Shade::InputEvent Shade::InputEvent::CreateKeyPress(KeyCode key)
{
    InputEvent result;
    result.mType = InputEventType::KEY;
    result.mKeyEvent = KeyEventType::PRESS;
    result.mKeyCode = key;
    return result;
}

// ======================================
Shade::InputEvent Shade::InputEvent::CreateKeyRelease(KeyCode key)
{
    InputEvent result;
    result.mType = InputEventType::KEY;
    result.mKeyEvent = KeyEventType::RELEASE;
    result.mKeyCode = key;
    return result;
}