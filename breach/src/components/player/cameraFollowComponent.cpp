#include "cameraFollowComponent.h"

#include "shade/instance/service/provider.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/camera/camera.h"

#include "components/facingComponent.h"
#include "map/mapService.h"

#include <algorithm>

namespace {
    constexpr float maxSpeed = 500.f;
    constexpr float speedMultiplier = 2.5f;

    constexpr float cameraLookahead = 150.f;
}

// ======================================
void CameraFollowComponent::Update(float deltaSeconds) {
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    MapService* map = Shade::ServiceProvider::GetCurrentProvider()->GetService<MapService>();
    const MapLayout& layout = map->GetLayout();
    
    const float currentPosition = camera->GetCameraInfo().x;
    float target = mEntityRef->GetPositionX();
    if (FacingComponent* facing = mEntityRef->GetComponent<FacingComponent>())
    {
        target += facing->mDirection == FacingDirection::RIGHT ? cameraLookahead : -cameraLookahead;
    }

    // No need to update any positions if the camera is already at the target position
    if (currentPosition == target)
    {
        return;
    }

    // TODO: These shouldn't be hard coded numbers for camera width
    const float adjustedMinX = layout.GetCameraMinX() + 640;
    const float adjustedMaxX = layout.GetCameraMaxX() - 640;
    const float speed = std::clamp(target - currentPosition, -maxSpeed, maxSpeed);
    const float newPosition = std::clamp(currentPosition + speed * deltaSeconds * speedMultiplier, adjustedMinX, adjustedMaxX);

    // TODO: y position update should come from a better place
    //  - Perhaps this can come from the map? idk..
    camera->SetCameraPosition(newPosition, 360.f);
}