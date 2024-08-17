#include "window.h"

// ======================================
Shade::ImGuiWindow::ImGuiWindow(const std::string& name)
    : mName(name)
{
    
}

// ======================================
const std::string& Shade::ImGuiWindow::GetName() const
{
    return mName;
}