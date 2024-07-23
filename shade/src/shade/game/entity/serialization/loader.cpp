#include "loader.h"

#include "shade/file/fileSystem.h"
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
        // TODO: There really should be some error-proofing here to prevent infinite loops
        //  - Maybe implement as a utility function w/ lambda callback?
        handle.ToNext();
    }

    return true;
}
