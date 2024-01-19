#pragma once

#include <optional>
#include <queue>

#include "shade/input/event.h"
#include "shade/instance/notifier.h"

namespace Shade {

    class InputHandler : public GameInstanceNotifier {
    public:
        InputHandler(GameInstance& instance);

        void Update();
        std::optional<InputEvent> GetNextEvent();

    private:
        std::queue<InputEvent> mEvents;
    };

}