#include "component.h"

// ======================================
Shade::Component::Component()
{

}

// ======================================
void Shade::Component::SetEntityRef(Entity* entityRef)
{
    // TODO: Maybe we should check here if entityRef is already set
    //  - We should never expect the entity of a component to change
    //  - If that assumption changes, then we would not need a check here
    mEntityRef = entityRef;
}

// ======================================
Shade::Component::~Component() = default;

// ======================================
void Shade::Component::Update(float deltaSeconds)
{
    // Not all components have an update function, so base version is empty function
}