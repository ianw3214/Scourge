#include "loader.h"

#include "shade/file/fileSystem.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/graphics/common.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
Shade::EntityLoaderService::EntityLoaderService()
    : Shade::Service("EntityLoaderService")
{

}

// ======================================
Shade::EntityLoaderService::~EntityLoaderService()
{

}

// ======================================
bool Shade::EntityLoaderService::LoadEntityFromFile(Shade::Entity& entity, const std::string& filePath)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    Shade::FileSystem* fileSystem = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::FileSystem>();
    std::unique_ptr<Shade::KeyValueFile> file = fileSystem->LoadKeyValueFile(filePath);

    if (file == nullptr)
    {
        logger->LogError(std::string("Could not load Entity, file was not loaded: ") + filePath);
        return false;
    }

    Shade::KeyValueHandle handle = file->GetContents();
    while(handle.IsValid())
    {
        // TODO: Can probably combine get key + type check to a single function
        if (handle.GetKey() == "name")
        {
            if (handle.IsString())
            {
                entity.SetName(handle.GetString());
                logger->LogInfo(std::string("Loading entity: ") + handle.GetString());
            }
            else
            {
                logger->LogWarning("Expected string for field 'name'");
            }
        }
        else if (handle.IsList())
        {
            const std::string& key = handle.GetKey();
            if (mComponentLoaders.find(key) == mComponentLoaders.end())
            {
                logger->LogError(std::string("No component loader found for: ") + key);
            }   
            else
            {
                entity.AddComponent(std::unique_ptr<Shade::Component>(mComponentLoaders[key](handle.GetListHead())));
            }
        }
        // TODO: There really should be some error-proofing here to prevent infinite loops
        //  - Maybe implement as a utility function w/ lambda callback?
        handle.ToNext();
    }

    logger->LogInfo(std::string("Successfully loaded entity: ") + filePath);
    return true;
}

// ======================================
void Shade::EntityLoaderService::RegisterComponentLoader(const std::string& name, ComponentLoader loader)
{
    if (mComponentLoaders.find(name) != mComponentLoaders.end())
    {
        Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        logger->LogError(std::string("Component loader already registered: ") + name);
        return;
    }

    mComponentLoaders[name] = loader;
}

// ======================================
void Shade::EntityLoaderService::LoadDefaultComponentLoaders()
{
    RegisterComponentLoader("sprite", [](Shade::KeyValueHandle handle) {
        Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        std::string texturePath = "";
        constexpr int renderLayer = 0;  // TODO: Does this need to actually be loadable from file?
        Shade::RenderAnchor renderAnchor = RenderAnchor::MIDDLE;

        while (handle.IsValid())
        {
            if (handle.GetKey() == "path")
            {
                if (handle.IsString())
                {
                    texturePath = handle.GetString();
                }
                else
                {
                    logger->LogWarning("Expected string for field 'texturePath'");
                }
            }
            if (handle.GetKey() == "anchor")
            {
                if (handle.IsString())
                {
                    renderAnchor = RenderUtil::StringToRenderAnchor(handle.GetString());
                }
                else
                {
                    logger->LogWarning("Expected string for field 'anchor'");
                }
            }
            handle.ToNext();
        }

        return new SpriteComponent(texturePath, renderLayer, renderAnchor);
    });
}