#include "service.h"

// ======================================
Shade::Service::Service(const std::string& name)
    : mName(name)
{

}

// ======================================
Shade::Service::~Service() = default;

// ======================================
const std::string& Shade::Service::GetName() const
{
    return mName;
}