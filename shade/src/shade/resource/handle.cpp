#include "handle.h"

Shade::ResourceHandle Shade::ResourceHandle::Invalid = Shade::ResourceHandle(-1, -1);

// ======================================
Shade::ResourceHandle::ResourceHandle(size_t Index, uint32_t Id)
    : mIndex(Index)
    , mId(Id)
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
