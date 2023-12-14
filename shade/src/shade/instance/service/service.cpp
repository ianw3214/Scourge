#include "service.h"

// ======================================
Shade::Service::Service(const std::string& Name)
    : mName(Name)
{

}

// ======================================
Shade::Service::~Service() = default;

// ======================================
const std::string& Shade::Service::GetName() const
{
    return mName;
}