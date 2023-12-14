#include "resource.h"

namespace ResourceGlobals {
    uint32_t NextResourceID = 0;
}

// ======================================
Shade::Resource::Resource()
    : mId(ResourceGlobals::NextResourceID++)
{

}

// ======================================
Shade::Resource::~Resource()
{
    
}

// ======================================
uint32_t Shade::Resource::GetResourceID() const
{
    return mId;
}