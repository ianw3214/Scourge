#pragma once

namespace Shade {

    class ImGuiWindow {
    public:
        ImGuiWindow();

        virtual void Draw() = 0;
    };

}