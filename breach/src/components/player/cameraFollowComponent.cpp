#include "cameraFollowComponent.h"

#include "shade/instance/service/provider.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/camera/camera.h"

// ======================================
void CameraFollowComponent::Update(float deltaSeconds) {
    Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
    // TODO: y position update should come from a better place
    camera->SetCameraPosition(mEntityRef->GetPositionX(), 360.f);
}