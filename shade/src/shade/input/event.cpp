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

// ======================================
Shade::InputEvent Shade::InputEvent::CreateMousePress(float x, float y)
{
    InputEvent result;
    result.mType = InputEventType::MOUSE;
    result.mMouseEvent = MouseEventType::PRESS;
    result.mMouseX = x;
    result.mMouseY = y;
    return result;
}

// ======================================
Shade::InputEvent Shade::InputEvent::CreateMouseRelease(float x, float y)
{
    InputEvent result;
    result.mType = InputEventType::MOUSE;
    result.mMouseEvent = MouseEventType::RELEASE;
    result.mMouseX = x;
    result.mMouseY = y;
    return result;
}

// ======================================
Shade::InputEvent Shade::InputEvent::CreateMouseMotion(float x, float y, float relX, float relY)
{
    InputEvent result;
    result.mType = InputEventType::MOUSE;
    result.mMouseEvent = MouseEventType::MOTION;
    result.mMouseX = x;
    result.mMouseY = y;
    result.mRelativeMouseX = relX;
    result.mRelativeMouseY = relY;
    return result;
}


// ======================================
Shade::InputEvent Shade::InputEvent::CreateButtonPress(ControllerButton button)
{
    InputEvent result;
    result.mType = InputEventType::CONTROLLER_BUTTON;
    result.mControllerButtonEvent = ButtonEventType::PRESS;
    result.mControllerButton = button;
    return result;
}

// ======================================
Shade::InputEvent Shade::InputEvent::CreateButtonRelease(ControllerButton button)
{
    InputEvent result;
    result.mType = InputEventType::CONTROLLER_BUTTON;
    result.mControllerButtonEvent = ButtonEventType::RELEASE;
    result.mControllerButton = button;
    return result;
}

// ======================================
Shade::InputEvent Shade::InputEvent::CreateAxisEvent(ControllerAxis axis, int32_t value)
{
    InputEvent result;
    result.mType = InputEventType::CONTROLLER_AXIS;
    result.mControllerAxis = axis;
    result.mControllerValue = value;
    return result;
}
