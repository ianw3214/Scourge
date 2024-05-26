#pragma once

#include "shade/graphics/imgui/window.h"

namespace Shade {

    class EditorModule;

    class EditorOverviewWindow : public ImGuiWindow {
    public:
        EditorOverviewWindow();

        void Draw() override;
    };

}