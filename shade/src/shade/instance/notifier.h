#pragma once

#include "shade/instance/notification.h"

namespace Shade {

    class GameInstance;

    class GameInstanceNotifier {
    public:
        GameInstanceNotifier(GameInstance& Instance);

        void NotifyGame(GameNotification Notification);
    private:
        GameInstance& mGameInstanceRef;
    };

}