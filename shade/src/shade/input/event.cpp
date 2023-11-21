#include "event.h"

// ======================================
Shade::InputEvent Shade::InputEvent::CreateKeyPress(KeyCode Key)
{
    InputEvent Result;
    Result.mType = InputEventType::KEY;
    Result.mKeyEvent = KeyEventType::PRESS;
    Result.mKeyCode = Key;
    return Result;
}

// ======================================
Shade::InputEvent Shade::InputEvent::CreateKeyRelease(KeyCode Key)
{
    InputEvent Result;
    Result.mType = InputEventType::KEY;
    Result.mKeyEvent = KeyEventType::RELEASE;
    Result.mKeyCode = Key;
    return Result;
}