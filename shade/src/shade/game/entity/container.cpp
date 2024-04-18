#include "container.h"

#include "shade/game/entity/entity.h"

// ======================================
std::unique_ptr<Shade::Entity>& Shade::EntityContainer::AddEntity(std::unique_ptr<Entity> entity)
{
    mEntities.emplace_back(std::move(entity));
    return mEntities.back();
}

// ======================================
std::vector<std::unique_ptr<Shade::Entity>>& Shade::EntityContainer::GetEntities()
{
    return mEntities;
}