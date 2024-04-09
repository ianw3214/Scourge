#pragma once

#include <vector>
#include <memory>

namespace Shade {

    class Entity;

    class EntityContainer {
    public:
        void AddEntity(std::unique_ptr<Entity> entity);
        std::vector<std::unique_ptr<Entity>>& GetEntities();

    protected:
        std::vector<std::unique_ptr<Entity>> mEntities;
    };

}