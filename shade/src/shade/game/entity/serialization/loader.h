#pragma once

#include "shade/game/entity/entity.h"
#include "shade/game/entity/component/component.h"
#include "shade/instance/service/service.h"
#include "shade/file/keyValueFile.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Shade {

    // ======================================
    // The entity loader service 
    //  - Handles loading entities from file
    class EntityLoaderService : public Shade::Service {
#ifdef BUILD_SHADE_EDITOR
    public:
        std::unique_ptr<Shade::Component> ShowNewComponentPopup();
#endif
    public:
        // The handle passed into the component loader should be the top level list handle
        typedef std::function<Shade::Component*(Shade::KeyValueHandle)> ComponentLoader;
    public:
        EntityLoaderService();
        ~EntityLoaderService();

        bool LoadEntityFromFile(Shade::Entity& entity, const std::string& filePath);

        void RegisterComponentLoader(const std::string& name, ComponentLoader loader);
        void LoadDefaultComponentLoaders();

    private:
        std::unordered_map<std::string, ComponentLoader> mComponentLoaders;

    };

}