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
bool Shade::ServiceProvider::RegisterService(Service* NewService)
{
    // TODO: Assert that 'NewService' is not nullptr
    for (std::unique_ptr<Service>& ServiceIt : mServices)
    {
        if (ServiceIt->GetName() == NewService->GetName())
        {
            return false;
        }
    }
    mServices.emplace_back(std::unique_ptr<Service>(NewService));
    return true;
}

// ======================================
void Shade::ServiceProvider::RegisterProvider(ServiceProvider* Provider)
{
    sProvider = Provider;
}

// ======================================
Shade::ServiceProvider* Shade::ServiceProvider::GetCurrentProvider()
{
    return sProvider;
}