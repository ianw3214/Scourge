#pragma once

#include <memory>
#include <string>

#include "shade/file/textFile.h"
#include "shade/file/keyValueFile.h"
#include "shade/instance/service/service.h"

namespace Shade {

    class FileSystem : public Service {
    public:
        FileSystem();

        std::unique_ptr<TextFile> LoadTextFile(const std::string& path);
        std::unique_ptr<KeyValueFile> LoadKeyValueFile(const std::string& path);
    };

}