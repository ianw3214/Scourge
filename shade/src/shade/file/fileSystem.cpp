#include "fileSystem.h"

#include <fstream>

// ======================================
Shade::FileSystem::FileSystem()
    : Service("FileSystem")
{
    
}

// ======================================
std::unique_ptr<Shade::TextFile> Shade::FileSystem::LoadTextFile(const std::string& path)
{
    std::ifstream rawFile(path);
    if (rawFile.is_open())
    {
        return TextFile::LoadFile(rawFile);
    }
    else
    {
        // TODO: Error...
    }

    return nullptr;
}

// ======================================
std::unique_ptr<Shade::KeyValueFile> Shade::FileSystem::LoadKeyValueFile(const std::string& path)
{
    std::ifstream rawFile(path);
    if (rawFile.is_open())
    {
        return KeyValueFile::LoadFile(rawFile);
    }
    else
    {
        // TODO: Error...
    }

    return nullptr;
}

// ======================================
bool Shade::FileSystem::SaveKeyValueFile(const std::string& path, const KeyValueFile& fileData)
{
    std::ofstream fileStream(path);
    if (fileStream.is_open())
    {
        return fileData.SaveFile(fileStream);
    }
    else
    {
        // TODO: Error...
    }

    return false;
}