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

    return true;
}
