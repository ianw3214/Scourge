#pragma once

#include <SDL2/SDL.h>

namespace Shade {

    // This needs a better solution if we allow non-SDL windowing
    enum class KeyCode : SDL_Keycode
    {
        SHADE_KEY_A = SDLK_a,
        SHADE_KEY_B = SDLK_b,
        SHADE_KEY_C = SDLK_c,
        SHADE_KEY_D = SDLK_d,
        SHADE_KEY_E = SDLK_e,
        SHADE_KEY_F = SDLK_f,
        SHADE_KEY_G = SDLK_g,
        SHADE_KEY_H = SDLK_h,
        SHADE_KEY_I = SDLK_i,
        SHADE_KEY_J = SDLK_j,
        SHADE_KEY_K = SDLK_k,
        SHADE_KEY_L = SDLK_l,
        SHADE_KEY_M = SDLK_m,
        SHADE_KEY_N = SDLK_n,
        SHADE_KEY_O = SDLK_o,
        SHADE_KEY_P = SDLK_p,
        SHADE_KEY_Q = SDLK_q,
        SHADE_KEY_R = SDLK_r,
        SHADE_KEY_S = SDLK_s,
        SHADE_KEY_T = SDLK_t,
        SHADE_KEY_U = SDLK_u,
        SHADE_KEY_V = SDLK_v,
        SHADE_KEY_W = SDLK_w,
        SHADE_KEY_X = SDLK_x,
        SHADE_KEY_Y = SDLK_y,
        SHADE_KEY_Z = SDLK_z,
    };

}