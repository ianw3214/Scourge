#include "notifier.h"

#include "shade/instance/instance.h"
#include "shade/graphics/command/command.h"

// ======================================
Shade::GameInstanceNotifier::GameInstanceNotifier(GameInstance& Instance)
    : mGameInstanceRef(Instance)
{

}

// ======================================
Shade::GameInstanceNotifier::~GameInstanceNotifier() = default;

// ======================================
void Shade::GameInstanceNotifier::NotifyGame(GameNotification Notification)
{
    mGameInstanceRef.Notify(Notification);
}

// ======================================
void Shade::GameInstanceNotifier::NotifyRenderer(std::unique_ptr<RenderCommand> Command)
{
    mGameInstanceRef.NotifyRenderer(std::move(Command));
}