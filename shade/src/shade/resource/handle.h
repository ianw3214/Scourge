#pragma once

#include <cstdint>

namespace Shade {

    class ResourceHandle {
    public:
        constexpr ResourceHandle(size_t index, uint32_t id);

        size_t GetIndex() const;
        uint32_t GetID() const;

        static const ResourceHandle Invalid;
    private:
        size_t mIndex;
        uint32_t mId;
    };

}