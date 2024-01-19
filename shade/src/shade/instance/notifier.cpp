#include "notifier.h"

#include "shade/instance/instance.h"
#include "shade/graphics/command/command.h"

// ======================================
Shade::GameInstanceNotifier::GameInstanceNotifier(GameInstance& instance)
    : mGameInstanceRef(instance)
{

}

// ======================================
Shade::GameInstanceNotifier::~GameInstanceNotifier() = default;

// ======================================
void Shade::GameInstanceNotifier::NotifyGame(GameNotification notification)
{
    mGameInstanceRef.Notify(notification);
}

// ======================================
void Shade::GameInstanceNotifier::NotifyRenderer(std::unique_ptr<RenderCommand> command)
{
    mGameInstanceRef.NotifyRenderer(std::move(command));
}