#pragma once

#include <string>
#include <vector>

namespace Shade {

    // For now, a file is stored as a vector of strings
    //  - may want to extend this in the future to handle other file types
    class File {
    public:
        File(std::vector<std::string>&& contents);

        const std::vector<std::string>& GetContents() const;
    private:
        std::vector<std::string> mContents;
    };

}