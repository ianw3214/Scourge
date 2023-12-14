#pragma once

#include <cstdint>

namespace Shade {

    class ResourceHandle {
    public:
        ResourceHandle(size_t Index, uint32_t Id);

        size_t GetIndex() const;
        uint32_t GetID() const;

        static ResourceHandle Invalid;
    private:
        size_t mIndex;
        uint32_t mId;
    };

}