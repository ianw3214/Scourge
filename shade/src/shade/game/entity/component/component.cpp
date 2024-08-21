#include "component.h"

#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

#include <cassert>

// ======================================
Shade::Component::Component()
{

}

// ======================================
void Shade::Component::SetEntityRef(Entity* entityRef)
{
    if (mEntityRef != nullptr)
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        logService->LogError("Component already attached to existing entity");
        return;
    }
    mEntityRef = entityRef;
}

// ======================================
Shade::Component::~Component() = default;

// ======================================
void Shade::Component::Initialize()
{
    assert(!mInitialized && "Components should only be initialized once");
    mInitialized = true;
}

// ======================================
void Shade::Component::Update(float deltaSeconds)
{
    // Not all components have an update function, so base version is empty function
}