#pragma once

#include "shade/game/entity/component/component.h"
#include "shade/game/entity/entity.h"
#include "shade/graphics/camera/camera.h"
#include "shade/instance/service/provider.h"

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
        mEntityRef->SetPositionX(camera->GetCameraInfo().x - mParallaxFactor * (camera->GetCameraInfo().x));
        // Assume the sprite for parallax components always have anchor set to the bottom middle
        mEntityRef->SetPositionY(0.f);
    }
    private:
    float mParallaxFactor = 1.0f;
};