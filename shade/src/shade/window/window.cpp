#include "window.h"

#include <SDL2/SDL.h>
#include <gl/glew.h>

#include "shade/graphics/imgui/wrapper.h"
#include "shade/graphics/imgui/service.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

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
    LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();

    mWindowImpl = std::make_unique<Window::WindowImpl>();
    mWindowImpl->mWindow = SDL_CreateWindow("Breach", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    if (mWindowImpl->mWindow == nullptr)
    {
        logService->LogError("Failed to create openGL window");
        return;
    }
    mWindowImpl->mContext = SDL_GL_CreateContext(mWindowImpl->mWindow);
    if (mWindowImpl->mContext == nullptr)
    {
        logService->LogError("Failed to create openGL context");
        return;
    }

    // Initialize GLEW
	GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        logService->LogError("Failed to initialize GLEW, error code: " + std::to_string(glewError));
        return;
    }

    // Initialize ImGui
    ImGuiWrapper::Initialize(mWindowImpl->mWindow, mWindowImpl->mContext);
}

// ======================================
Shade::Window::~Window()
{
    
}   

// ======================================
void Shade::Window::Update()
{
    ImGuiService* imgui = ServiceProvider::GetCurrentProvider()->GetService<ImGuiService>();
    imgui->DrawWindows();

    ImGuiWrapper::EndFrame();
    SDL_GL_SwapWindow(mWindowImpl->mWindow);
}