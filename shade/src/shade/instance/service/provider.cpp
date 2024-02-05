#include "provider.h"

#include "shade/instance/service/service.h"

// ======================================
Shade::ServiceProvider::ServiceProvider()
{

}

// ======================================
Shade::ServiceProvider::~ServiceProvider()
{

}

// ======================================
bool Shade::ServiceProvider::RegisterService(Service* service)
{
    // TODO: Assert that 'service' is not nullptr
    for (std::unique_ptr<Service>& serviceIT : mServices)
    {
        if (serviceIT->GetName() == service->GetName())
        {
            return false;
        }
    }
    mServices.emplace_back(std::unique_ptr<Service>(service));
    return true;
}

// ======================================
void Shade::ServiceProvider::RegisterProvider(ServiceProvider* provider)
{
    sProvider = provider;
}

// ======================================
Shade::ServiceProvider* Shade::ServiceProvider::GetCurrentProvider()
{
    return sProvider;
}