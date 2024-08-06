#pragma once

#include <string>

namespace Shade {

    class ImGuiWindow {
    public:
        ImGuiWindow(const std::string& name);

        const std::string& GetName() const;

        virtual void Draw() = 0;
    private:
        std::string mName;
    };

}