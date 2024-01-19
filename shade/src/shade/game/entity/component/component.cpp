#include "component.h"

// ======================================
Shade::Component::Component(Entity& owner)
    : mEntityRef(owner)
{

}

// ======================================
Shade::Component::~Component() = default;

// ======================================
void Shade::Component::Update(float deltaSeconds)
{
    // Not all components have an update function, so base version is empty function
}