#pragma once

#include <memory>
#include <string>

#include "shade/file/jsonFile.h"
#include "shade/file/textFile.h"
#include "shade/file/keyValueFile.h"
#include "shade/instance/service/service.h"

namespace Shade {

    // The file system is a centralized service place to load/save files
    //  - Files typically are read and then discarded
    //  - As such, there is no mechanism for retaining file resources provided by the system
    //  - Ownership is transferred to the caller when loading a file
    class FileSystem : public Service {
    public:
        FileSystem();

        std::unique_ptr<TextFile> LoadTextFile(const std::string& path);
        std::unique_ptr<KeyValueFile> LoadKeyValueFile(const std::string& path);
        std::unique_ptr<JsonFile> LoadJsonFile(const std::string& path);

        bool SaveKeyValueFile(const std::string& path, const KeyValueFile& fileData);
    };

}