#include "component.h"

// ======================================
Shade::Component::Component(Entity& Owner)
    : mEntityRef(Owner)
{

}

// ======================================
Shade::Component::~Component() = default;

// ======================================
void Shade::Component::Update(float DeltaSeconds)
{
    // Not all components have an update function, so base version is empty function
}