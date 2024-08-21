#pragma once

#include "shade/instance/service/service.h"
#include "shade/graphics/imgui/window.h"

#include <memory>
#include <vector>

namespace Shade {

    class ImGuiService : public Service {
    public:
        ImGuiService();
        ~ImGuiService();

        void RegisterWindow(std::unique_ptr<ImGuiWindow>&& window);
        void TryRegisterWindow(std::unique_ptr<ImGuiWindow>&& window);
        void DeleteWindow(const std::string& windowName);

        void DrawWindows();
        void AddQueuedWindows();
        void FlushDeletedWindows();
    private:
        std::vector<std::unique_ptr<ImGuiWindow>> mWindows;

        // Vectors to queue addition/deletion of windows to avoid resource access errors
        std::vector<std::unique_ptr<ImGuiWindow>> mWindowsToAdd;
        std::vector<std::string> mWindowsToDelete;
    };

}