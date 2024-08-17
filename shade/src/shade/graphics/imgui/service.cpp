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
    mWindowsToAdd.emplace_back(std::move(window));
}

// ======================================
void Shade::ImGuiService::DeleteWindow(const std::string& windowName)
{
    mWindowsToDelete.emplace_back(windowName);
}

// ======================================
void Shade::ImGuiService::DrawWindows()
{
    for (std::unique_ptr<ImGuiWindow>& window : mWindows)
    {
        window->Draw();
    }
}

// ======================================
void Shade::ImGuiService::AddQueuedWindows()
{
    for (std::unique_ptr<ImGuiWindow>& window : mWindowsToAdd)
    {
        // TODO: Check if window already exists
        mWindows.emplace_back(std::move(window));
    }
    mWindowsToAdd.clear();
}

// ======================================
void Shade::ImGuiService::FlushDeletedWindows()
{
    for (const std::string& windowName : mWindowsToDelete)
    {
        for (auto it = mWindows.begin(); it != mWindows.end(); it++)
        {
            if (it->get()->GetName() == windowName)
            {
                mWindows.erase(it);
                break;
            }
        }
        // TODO: Log error or warning if nothing was deleted
    }
    
    mWindowsToDelete.clear();
}