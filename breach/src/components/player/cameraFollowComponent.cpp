#include "cameraFollowComponent.h"

#include "shade/instance/service/provider.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/camera/camera.h"

#include <algorithm>

namespace {
    constexpr float maxSpeed = 500.f;
    constexpr float speedMultiplier = 2.5f;
}

// ======================================
void CameraFollowComponent::Update(float deltaSeconds) {
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    
    const float currentPosition = camera->GetCameraInfo().x;
    if (currentPosition == mEntityRef->GetPositionX())
    {
        return;
    }

    const float speed = std::clamp(mEntityRef->GetPositionX() - currentPosition, -maxSpeed, maxSpeed);
    const float newPosition = currentPosition + speed * deltaSeconds * speedMultiplier;

    // TODO: y position update should come from a better place
    //  - Perhaps this can come from the map? idk..
    camera->SetCameraPosition(newPosition, 360.f);
}