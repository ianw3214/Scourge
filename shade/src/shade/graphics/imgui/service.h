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

        void DrawWindows();
    private:
        std::vector<std::unique_ptr<ImGuiWindow>> mWindows;
    };

}