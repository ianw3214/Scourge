#pragma once

#include <SDL2/SDL.h>

namespace Shade {

    // TODO: This needs a better solution if we allow non-SDL windowing
    enum class KeyCode : uint16_t
    {
        SHADE_KEY_A = SDL_SCANCODE_A,
        SHADE_KEY_B = SDL_SCANCODE_B,
        SHADE_KEY_C = SDL_SCANCODE_C,
        SHADE_KEY_D = SDL_SCANCODE_D,
        SHADE_KEY_E = SDL_SCANCODE_E,
        SHADE_KEY_F = SDL_SCANCODE_F,
        SHADE_KEY_G = SDL_SCANCODE_G,
        SHADE_KEY_H = SDL_SCANCODE_H,
        SHADE_KEY_I = SDL_SCANCODE_I,
        SHADE_KEY_J = SDL_SCANCODE_J,
        SHADE_KEY_K = SDL_SCANCODE_K,
        SHADE_KEY_L = SDL_SCANCODE_L,
        SHADE_KEY_M = SDL_SCANCODE_M,
        SHADE_KEY_N = SDL_SCANCODE_N,
        SHADE_KEY_O = SDL_SCANCODE_O,
        SHADE_KEY_P = SDL_SCANCODE_P,
        SHADE_KEY_Q = SDL_SCANCODE_Q,
        SHADE_KEY_R = SDL_SCANCODE_R,
        SHADE_KEY_S = SDL_SCANCODE_S,
        SHADE_KEY_T = SDL_SCANCODE_T,
        SHADE_KEY_U = SDL_SCANCODE_U,
        SHADE_KEY_V = SDL_SCANCODE_V,
        SHADE_KEY_W = SDL_SCANCODE_W,
        SHADE_KEY_X = SDL_SCANCODE_X,
        SHADE_KEY_Y = SDL_SCANCODE_Y,
        SHADE_KEY_Z = SDL_SCANCODE_Z,

        SHADE_KEY_SPACE = SDL_SCANCODE_SPACE,
        SHADE_KEY_LSHIFT = SDL_SCANCODE_LSHIFT,
        SHADE_KEY_LCONTROL = SDL_SCANCODE_LCTRL,
        SHADE_KEY_LALT = SDL_SCANCODE_LALT,

        SHADE_KEY_UP = SDL_SCANCODE_UP,
        SHADE_KEY_DOWN = SDL_SCANCODE_DOWN,
        SHADE_KEY_LEFT = SDL_SCANCODE_LEFT,
        SHADE_KEY_RIGHT = SDL_SCANCODE_RIGHT,

        SHADE_KEY_F1 = SDL_SCANCODE_F1,
        SHADE_KEY_F2 = SDL_SCANCODE_F2,
        SHADE_KEY_F3 = SDL_SCANCODE_F3,
        SHADE_KEY_F4 = SDL_SCANCODE_F4,
        SHADE_KEY_F5 = SDL_SCANCODE_F5,
        SHADE_KEY_F6 = SDL_SCANCODE_F6
    };

    // Controller buttons
    enum class ControllerButton : int8_t
    {
        SHADE_BUTTON_INVALID = SDL_CONTROLLER_BUTTON_INVALID,
        SHADE_BUTTON_A = SDL_CONTROLLER_BUTTON_A,
        SHADE_BUTTON_B = SDL_CONTROLLER_BUTTON_B,
        SHADE_BUTTON_X = SDL_CONTROLLER_BUTTON_X,
        SHADE_BUTTON_Y = SDL_CONTROLLER_BUTTON_Y,
        SHADE_BUTTON_BACK = SDL_CONTROLLER_BUTTON_BACK,
        SHADE_BUTTON_GUIDE = SDL_CONTROLLER_BUTTON_GUIDE,
        SHADE_BUTTON_START = SDL_CONTROLLER_BUTTON_START,
        SHADE_BUTTON_LEFTSTICK = SDL_CONTROLLER_BUTTON_LEFTSTICK,
        SHADE_BUTTON_RIGHTSTICK = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
        SHADE_BUTTON__BUTTON_LEFTSHOULDER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
        SHADE_BUTTON__BUTTON_RIGHTSHOULDER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
        SHADE_BUTTON__BUTTON_DPAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
        SHADE_BUTTON__BUTTON_DPAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
        SHADE_BUTTON__BUTTON_DPAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
        SHADE_BUTTON__BUTTON_DPAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        SHADE_BUTTON__BUTTON_MISC1 = SDL_CONTROLLER_BUTTON_MISC1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
        SHADE_BUTTON__BUTTON_PADDLE1 = SDL_CONTROLLER_BUTTON_PADDLE1,  /* Xbox Elite paddle P1 (upper left, facing the back) */
        SHADE_BUTTON__BUTTON_PADDLE2 = SDL_CONTROLLER_BUTTON_PADDLE2,  /* Xbox Elite paddle P3 (upper right, facing the back) */
        SHADE_BUTTON__BUTTON_PADDLE3 = SDL_CONTROLLER_BUTTON_PADDLE3,  /* Xbox Elite paddle P2 (lower left, facing the back) */
        SHADE_BUTTON__BUTTON_PADDLE4 = SDL_CONTROLLER_BUTTON_PADDLE4,  /* Xbox Elite paddle P4 (lower right, facing the back) */
        SHADE_BUTTON__BUTTON_TOUCHPAD = SDL_CONTROLLER_BUTTON_TOUCHPAD, /* PS4/PS5 touchpad button */
    };

    // Controller Axis
    enum class ControllerAxis : int8_t
    {
        SHADE_AXIS_INVALID = SDL_CONTROLLER_AXIS_INVALID,
        SHADE_AXIS_LEFTX = SDL_CONTROLLER_AXIS_LEFTX,
        SHADE_AXIS_LEFTY = SDL_CONTROLLER_AXIS_LEFTY,
        SHADE_AXIS_RIGHTX = SDL_CONTROLLER_AXIS_RIGHTX,
        SHADE_AXIS_RIGHTY = SDL_CONTROLLER_AXIS_RIGHTY,
        SHADE_AXIS_TRIGGERLEFT = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
        SHADE_AXIS_TRIGGERRIGHT = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
        SHADE_AXIS_MAX = SDL_CONTROLLER_AXIS_MAX
    };

}