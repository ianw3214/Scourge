#include "notifier.h"

#include "shade/instance/instance.h"

// ======================================
Shade::GameInstanceNotifier::GameInstanceNotifier(GameInstance& Instance)
    : mGameInstanceRef(Instance)
{

}

// ======================================
void Shade::GameInstanceNotifier::NotifyGame(GameNotification Notification)
{
    mGameInstanceRef.Notify(Notification);
}