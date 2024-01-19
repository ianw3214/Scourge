#pragma once

#include <memory>

#include "shade/instance/notification.h"

namespace Shade {

    class GameInstance;
    class RenderCommand;

    class GameInstanceNotifier {
    public:
        GameInstanceNotifier(GameInstance& instance);
        ~GameInstanceNotifier();

        void NotifyGame(GameNotification notification);
        void NotifyRenderer(std::unique_ptr<RenderCommand> command);
    private:
        GameInstance& mGameInstanceRef;
    };

}