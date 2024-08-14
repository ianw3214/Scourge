#include "loader.h"

#include "shade/file/fileSystem.h"
#include "shade/game/entity/component/spriteComponent.h"
#include "shade/game/entity/component/animatedSpriteComponent.h"
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
    // Register sprite loading
    // TODO: Move these to sprite/animated sprite files to centralize save/load logic
    RegisterComponentLoader("sprite", [](Shade::KeyValueHandle handle) {
        Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        std::string texturePath = "";
        constexpr int renderLayer = 0;  // TODO: Does this need to actually be loadable from file?
        Shade::RenderAnchor renderAnchor = RenderAnchor::MIDDLE;

        while (handle.IsValid())
        {
            if (handle.GetKey() == "path")
            {
                texturePath = handle.TryGetString();
            }
            if (handle.GetKey() == "anchor")
            {
                renderAnchor = RenderUtil::StringToRenderAnchor(handle.TryGetString());
            }
            handle.ToNext();
        }

        return new SpriteComponent(texturePath, renderLayer, renderAnchor);
    });

    // Register animated sprited loading
    RegisterComponentLoader("animated_sprite", [](Shade::KeyValueHandle handle) {
        Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
        
        // TODO: Consider letting animated sprites use the default frame data render width/height
        float renderWidth = 100.f;
        float renderHeight = 100.f;
        std::string texturePath = "";
        std::string frameDataPath = "";
        std::string initialState = "";
        constexpr int renderLayer = 0;  // TODO: Does this need to actually be loadable from file?
        Shade::RenderAnchor renderAnchor = RenderAnchor::BOTTOM_MIDDLE;
        std::vector<std::pair<std::string, std::string>> transitions;

        while (handle.IsValid())
        {
            if (handle.GetKey() == "width")
            {
                renderWidth = handle.TryGetFloat();
            }
            if (handle.GetKey() == "height")
            {
                renderHeight = handle.TryGetFloat();
            }
            if (handle.GetKey() == "path")
            {
                texturePath = handle.TryGetString();
            }
            if (handle.GetKey() == "frame_data")
            {
                frameDataPath = handle.TryGetString();
            }
            if (handle.GetKey() == "initial_state")
            {
                initialState = handle.TryGetString();
            }
            if (handle.GetKey() == "anchor")
            {
                renderAnchor = RenderUtil::StringToRenderAnchor(handle.TryGetString());
            }
            if (handle.GetKey() == "transitions")
            {
                // TODO: Error check that this is a list
                KeyValueHandle transitionHandle = handle.GetListHead();
                while (transitionHandle.IsValid())
                {
                    // The key is directly used as the "from" animation, and the value is used for the "to" animation
                    const std::string& from = transitionHandle.GetKey();
                    const std::string& to = transitionHandle.TryGetString("");
                    transitions.emplace_back(from, to);
                    transitionHandle.ToNext();
                }
            }
            handle.ToNext();
        }

        return new AnimatedSpriteComponent(renderWidth, renderHeight, texturePath, frameDataPath, transitions, initialState, renderLayer, renderAnchor);;
    });
}