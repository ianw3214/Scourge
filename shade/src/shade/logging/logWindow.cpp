#include "logWindow.h"

#include <imgui.h>

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
void Shade::LogWindow::Draw()
{
    LogService* logs = ServiceProvider::GetCurrentProvider()->GetService<LogService>();

    ImGui::Begin("logs");
    for (const std::string& message : logs->getLogs())
    {
        ImGui::Text(message.c_str());
    }
    ImGui::End();
}
