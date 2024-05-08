#pragma once

#include <fstream>
#include <string>
#include <memory>
#include <vector>

namespace Shade {

    class TextFile {
    public:
        TextFile(std::vector<std::string>&& contents);

        static std::unique_ptr<TextFile> LoadFile(std::ifstream& fileStream);

        const std::vector<std::string>& GetContents() const;
    private:
        std::vector<std::string> mContents;
    };

}