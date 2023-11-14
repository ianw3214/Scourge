#pragma once

#include <memory>

#include "shade/instance/notification.h"

namespace Shade {

    class GameInstance;
    class RenderCommand;

    class GameInstanceNotifier {
    public:
        GameInstanceNotifier(GameInstance& Instance);
        ~GameInstanceNotifier();

        void NotifyGame(GameNotification Notification);
        void NotifyRenderer(std::unique_ptr<RenderCommand> Command);
    private:
        GameInstance& mGameInstanceRef;
    };

}