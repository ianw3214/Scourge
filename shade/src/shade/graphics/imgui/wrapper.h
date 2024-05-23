#pragma once

#include <SDL2/SDL.h>

namespace Shade {

    // ImGui doens't need to store any extra state so these are just namespace based utility functions
    namespace ImGuiWrapper {
        void Initialize(SDL_Window* window, void* gl_context);
        void Shutdown();

        bool HandleEvent(const SDL_Event* event);

        void StartFrame();
        void EndFrame();

        bool WantCaptureMouse();
        bool WantCaptureKeyboard();
    }

}