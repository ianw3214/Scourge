#pragma once

#include "shade/instance/service/service.h"

namespace Shade {

    struct CameraInfo {
        float x = 0.f;
        float y = 0.f;
        // TODO: Proper initialization
        float mOffsetX = 640.f;
        float mOffsetY = 360.f;
    };

    // The camera class handles positioning the virtual viewport in the game
    //  - The x/y position represent the center of the camera viewport
    //  - Custom camera behaviour should not be implemented in the base camera service
    class CameraService : public Service {
    public:
        CameraService();
        ~CameraService();

        const CameraInfo& GetCameraInfo() const;

    private:
        CameraInfo mCameraInfo;
    };

}