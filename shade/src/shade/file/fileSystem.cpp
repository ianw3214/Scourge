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