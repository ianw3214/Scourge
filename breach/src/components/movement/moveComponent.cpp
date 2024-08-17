#include "moveComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/instance/service/provider.h"

#include "map/mapService.h"

const std::string BaseMovementComponent::ComponentID = "movement";

namespace {

    // ======================================
    // TODO: Is there a better way of ray casting to find a good intersection point
    //  rather than just not allowing movement completely
    bool IsPositionInMapBounds(const Shade::Vec2& point)
    {
        MapService* map = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
        const MapLayout& layout = map->GetLayout();
        for (const Shade::Box& playZone : layout.GetPlayZones())
        {
            if (Shade::PointInBox(point, playZone))
            {
                return true;
            }
        }
        return false;
    }

}

// ======================================
void BaseMovementComponent::MoveLeft(float distance)
{
    // TODO: Centralize check of current entity position
    if (IsPositionInMapBounds(mEntityRef->GetPosition().Left(distance)) || !IsPositionInMapBounds(mEntityRef->GetPosition()))
    {
        mEntityRef->SetPositionX(mEntityRef->GetPositionX() - distance);
    }
}

// ======================================
void BaseMovementComponent::MoveRight(float distance)
{
    if (IsPositionInMapBounds(mEntityRef->GetPosition().Right(distance)) || !IsPositionInMapBounds(mEntityRef->GetPosition()))
    {
        mEntityRef->SetPositionX(mEntityRef->GetPositionX() + distance);
    }
}

// ======================================
void BaseMovementComponent::MoveUp(float distance)
{
    if (IsPositionInMapBounds(mEntityRef->GetPosition().Up(distance)) || !IsPositionInMapBounds(mEntityRef->GetPosition()))
    {
        mEntityRef->SetPositionY(mEntityRef->GetPositionY() + distance);
    }
}

// ======================================
void BaseMovementComponent::MoveDown(float distance)
{
    if (IsPositionInMapBounds(mEntityRef->GetPosition().Down(distance)) || !IsPositionInMapBounds(mEntityRef->GetPosition()))
    {
        mEntityRef->SetPositionY(mEntityRef->GetPositionY() - distance);
    }
}