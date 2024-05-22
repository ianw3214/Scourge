#pragma once

#include "shade/game/entity/component/component.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/camera/camera.h"
#include "shade/instance/service/provider.h"

namespace ParallaxUtil {

    inline float GetParallaxPos(float worldPos, float parallax, Shade::CameraService* camera)
    {
        // World position is an absolute offset from 0, so the camera position doesn't affect it
        return camera->GetCameraInfo().x - parallax * (camera->GetCameraInfo().x) + worldPos;
    }

}

// ======================================
class HorizontalParallaxComponent : public Shade::Component
{
public:
    // ======================================
    HorizontalParallaxComponent(float parallax, float x = 0.f) : mParallaxFactor(parallax), mWorldX(x) {}
    // ======================================
    void Update(float deltaSeconds) override {
        // Assume things with a parallax component just work from position x=0
        //  - if this needs to change, need to separate concept between world x/y and rendering x/y
        //  - or camera needs to somehow know to not use world x/y for parallax entities
        Shade::CameraService* camera = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::CameraService>();
        mEntityRef->SetPositionX(ParallaxUtil::GetParallaxPos(mWorldX, mParallaxFactor, camera));
        // Parallax doesn't affect vertical movement so just let the entity y be the world y
    }
    float GetWorldX() const { return mWorldX; }
    void SetWorldX(float x) { mWorldX = x; }
private:
    float mParallaxFactor = 1.0f;
    // The entity x/y is used for rendering
    // so the actual world position of the parallax component needs to be stored here
    float mWorldX = 0.f;
};