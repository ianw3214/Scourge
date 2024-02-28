#include "service.h"

// ======================================
Shade::ImGuiService::ImGuiService()
    : Service("ImGuiService")
{

}

// ======================================
Shade::ImGuiService::~ImGuiService() = default;

// ======================================
void Shade::ImGuiService::RegisterWindow(std::unique_ptr<ImGuiWindow>&& window)
{
    mWindows.emplace_back(std::move(window));
}

// ======================================
void Shade::ImGuiService::DrawWindows()
{
    for (std::unique_ptr<ImGuiWindow>& window : mWindows)
    {
        window->Draw();
    }
}