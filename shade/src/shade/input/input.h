#pragma once

#include "shade/instance/notifier.h"

namespace Shade {

    class InputHandler : public GameInstanceNotifier {
    public:
        InputHandler(GameInstance& Instance);

        void Update();
    };

}