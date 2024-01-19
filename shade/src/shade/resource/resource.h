#pragma once

#include <cstdint>
#include <string>

namespace Shade {

    class Resource {
    public:
        Resource();
        virtual ~Resource();

        uint32_t GetResourceID() const;

        // This should be implemented by any child class of a resource
        static Resource* Load(const std::string& path) = delete;
    private:
        const uint32_t mId;
    };

}