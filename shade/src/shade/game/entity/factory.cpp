#include "factory.h"

#include "shade/game/entity/entity.h"
#include "shade/game/entity/container.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

#include <cassert>

std::unique_ptr<Shade::Entity> Shade::EntityFactory::NullEntity = nullptr;

// ======================================
Shade::EntityFactory::EntityFactory() 
    : Shade::Service("EntityFactory")
{
    
}

// ======================================
void Shade::EntityFactory::RegisterEntityWorldInfo(EntityWorldInfo&& worldInfo)
{
    mEntityWorldInfo = std::make_unique<EntityWorldInfo>(worldInfo);
}

// ======================================
void Shade::EntityFactory::UnRegisterEntityWorldInfo()
{
    mEntityWorldInfo.reset();
}

// ======================================
std::unique_ptr<Shade::Entity> Shade::EntityFactory::CreateNewEntity()
{
    if (mEntityWorldInfo == nullptr)
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        logService->LogError("Tried to create new entity while no world info is registered");
        return nullptr;
    }
    return std::make_unique<Shade::Entity>(mEntityWorldInfo->mEventSource, mEntityWorldInfo->mEntityContainer);
}

// ======================================
std::unique_ptr<Shade::Entity>& Shade::EntityFactory::CreateAndRegisterNewEntity()
{
    std::unique_ptr<Shade::Entity> newEntity = CreateNewEntity();
    if (newEntity) 
    {
        return mEntityWorldInfo->mEntityContainer.AddEntity(std::move(newEntity));
    }
    LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
    logService->LogError("Unable to register null entity");
    return EntityFactory::NullEntity;
}