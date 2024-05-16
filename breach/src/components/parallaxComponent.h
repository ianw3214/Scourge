#pragma once

#include "shade/game/entity/component/component.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/camera/camera.h"
#include "shade/instance/service/provider.h"

namespace ParallaxUtil {

    // TODO: Currently worldPos is irrelevant as we are assuming all background entities have world position 0
    //  - This will probably have to update to actually account for world position
    inline float GetParallaxPos(float worldPos, float parallax, Shade::CameraService* camera)
    {
        return camera->GetCameraInfo().x - parallax * (camera->GetCameraInfo().x);
    }

}

// ======================================
class HorizontalParallaxComponent : public Shade::Component
{
    public:
    // ======================================
    HorizontalParallaxComponent(float parallax) : mParallaxFactor(parallax) {}
    // ======================================
    void Update(float deltaSeconds) override {
        // Assume things with a parallax component just work from position x=0
        //  - if this needs to change, need to separate concept between world x/y and rendering x/y
        //  - or camera needs to somehow know to not use world x/y for parallax entities
        Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
        mEntityRef->SetPositionX(ParallaxUtil::GetParallaxPos(mEntityRef->GetPositionX(), mParallaxFactor, camera));
        // Assume the sprite for parallax components always have anchor set to the bottom middle
        mEntityRef->SetPositionY(0.f);
    }
    private:
    float mParallaxFactor = 1.0f;
};