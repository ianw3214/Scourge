#pragma once

#include <memory>
#include <string>

#include "shade/file/file.h"
#include "shade/instance/service/service.h"

namespace Shade {

    class FileSystem : public Service {
    public:
        FileSystem();

        // TODO: Might want to be able to load different file types
        std::unique_ptr<File> LoadFile(const std::string& path);
    };

}