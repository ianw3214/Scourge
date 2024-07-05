#pragma once

#include <vector>
#include <memory>

namespace Shade {

    class Entity;

    class EntityContainer {
    public:
        std::unique_ptr<Entity>& AddEntity(std::unique_ptr<Entity> entity);
        std::vector<std::unique_ptr<Entity>>& GetEntities();

        void ClearAllEntities();
        void DeleteMarkedEntities();

    protected:
        std::vector<std::unique_ptr<Entity>> mEntities;
    };

}