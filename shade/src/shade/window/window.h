#pragma once

#include <memory>

namespace Shade {

    class Window {
    public:
        Window();
        ~Window();

        void Update();
    private:
        // factoring out impl to make it easier to handle different windowing methods
        struct WindowImpl;
        std::unique_ptr<WindowImpl> mWindowImpl;
    };

}