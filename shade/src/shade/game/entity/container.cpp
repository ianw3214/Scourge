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

// ======================================
void Shade::EntityContainer::DeleteMarkedEntities()
{
    mEntities.resize(std::distance(mEntities.begin(), std::remove_if(mEntities.begin(), mEntities.end(), [](const std::unique_ptr<Entity>& entity){
        return entity->IsMarkedForDelete();
    })));
}