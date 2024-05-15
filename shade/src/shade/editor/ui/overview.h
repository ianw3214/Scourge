#pragma once

#include "shade/graphics/imgui/window.h"

namespace Shade {

    class EditorModule;

    class EditorOverviewWindow : public ImGuiWindow {
    public:
        EditorOverviewWindow(EditorModule& editor);

        void Draw() override;
    private:
        EditorModule& mEditorRef;
    };

}