#include "fileSystem.h"

#include <fstream>

// ======================================
Shade::FileSystem::FileSystem()
    : Service("FileSystem")
{
    
}

// ======================================
std::unique_ptr<Shade::File> Shade::FileSystem::LoadFile(const std::string& path)
{
    std::ifstream rawFile(path);
    if (rawFile.is_open())
    {
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(rawFile, line))
        {
            lines.emplace_back(line);
        }
        return std::make_unique<File>(std::move(lines));
    }
    else
    {
        // TODO: Error...
    }

    return nullptr;
}