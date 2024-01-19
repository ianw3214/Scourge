#include "window.h"

#include <SDL2/SDL.h>
#include <gl/glew.h>

namespace Shade {
    // ======================================
    struct Window::WindowImpl {
    public:
        SDL_Window* mWindow;
        SDL_GLContext mContext;
    };
}

// ======================================
Shade::Window::Window()
    : mWindowImpl(nullptr)
{
    // TODO: Error handling
    mWindowImpl = std::make_unique<Window::WindowImpl>();
    mWindowImpl->mWindow = SDL_CreateWindow("Breach", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    mWindowImpl->mContext = SDL_GL_CreateContext(mWindowImpl->mWindow);

    // Initialize GLEW
	GLenum glewError = glewInit();
}

// ======================================
Shade::Window::~Window()
{

}   

// ======================================
void Shade::Window::Update()
{
    SDL_GL_SwapWindow(mWindowImpl->mWindow);
}