#pragma once

#include <nlohmann/single_include/nlohmann/json.hpp>

#include <fstream>
#include <string>
#include <vector>

namespace Shade {

    // ======================================
    class JsonFile {
    public:
        JsonFile(nlohmann::json&& jsonContents);

        static std::unique_ptr<JsonFile> LoadFile(std::ifstream& fileStream);
        bool SaveFile(std::ofstream& fileStream) const;

        const nlohmann::json& GetContents() const;

    protected:
        JsonFile() {}

    private:
        nlohmann::json mJsonContents;
    };

}