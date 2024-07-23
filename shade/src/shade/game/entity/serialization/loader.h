#pragma once

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/component.h"
#include "shade/instance/service/service.h"
#include "shade/file/keyValueFile.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace Shade {

    // ======================================
    // The entity loader service 
    //  - Handles loading entities from file
    class EntityLoaderService : public Shade::Service {
    public:
        EntityLoaderService();
        ~EntityLoaderService();

        bool LoadEntityFromFile(Shade::Entity& entity, const std::string& filePath);

    private:
        std::unordered_map<std::string, std::function<Shade::Component*(Shade::KeyValueHandle)>> mComponentLoaders;

    };

}