#include "handle.h"

Shade::ResourceHandle Shade::ResourceHandle::Invalid = Shade::ResourceHandle(-1, -1);

// ======================================
Shade::ResourceHandle::ResourceHandle(size_t index, uint32_t id)
    : mIndex(index)
    , mId(id)
{

}

// ======================================
size_t Shade::ResourceHandle::GetIndex() const
{
    return mIndex;
}

// ======================================
uint32_t Shade::ResourceHandle::GetID() const
{
    return mId;
}
