#pragma once

#include "shade/graphics/imgui/window.h"
#include "shade/logging/logger.h"

#include <vector>
#include <string>

namespace Shade {

    class LogWindow : public ImGuiWindow {
    public:
        LogWindow();
        
        void Draw() override;
    };

}