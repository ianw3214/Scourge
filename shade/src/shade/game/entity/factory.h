#pragma once

#include <memory>
#include <string>

#include "shade/instance/service/service.h"

namespace Shade {

    class Entity;
    class GameplayEventSource; 
    class EntityContainer;

    struct EntityWorldInfo {
        GameplayEventSource& mEventSource;
        EntityContainer& mEntityContainer;
    };

    // The entity factory allows different parts of the game to easily add new entitites to the current game world
    //  - New game worlds automatically register themselves to the entity factory so no manually registration is needed
    class EntityFactory : public Service {
        static std::unique_ptr<Entity> NullEntity;
    public:
        EntityFactory();

        void RegisterEntityWorldInfo(EntityWorldInfo&& worldInfo);
        void UnRegisterEntityWorldInfo();

        std::unique_ptr<Entity> CreateNewEntity(const std::string& filePath = "");
        std::unique_ptr<Entity>& CreateAndRegisterNewEntity(const std::string& filePath = "");
    private:
        std::unique_ptr<EntityWorldInfo> mEntityWorldInfo;
    };

}