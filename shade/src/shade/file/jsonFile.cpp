#include "jsonFile.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

// ======================================
Shade::JsonFile::JsonFile(nlohmann::json&& jsonContents)
    : mJsonContents(jsonContents)
{

}

// ======================================
std::unique_ptr<Shade::JsonFile> Shade::JsonFile::LoadFile(std::ifstream& fileStream)
{
    Shade::LogService* logger = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::LogService>();
    nlohmann::json jsonContents = nlohmann::json::parse(fileStream);
    return std::make_unique<JsonFile>(std::move(jsonContents));
}

// ======================================
bool Shade::JsonFile::SaveFile(std::ofstream& fileStream) const
{
    // TODO: Implement
    //  - Currently not used
    return false;
}

// ======================================
const nlohmann::json& Shade::JsonFile::GetContents() const
{
    return mJsonContents;
}