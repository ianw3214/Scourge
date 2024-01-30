#pragma once

#include "shade/instance/service/service.h"

namespace Shade {

    struct CameraInfo {
        float x = 0.f;
        float y = 0.f;
        // These offset values directly reflect the camera x/y position, but account for the screen width/height
        //  - Used to directly calculate the final position of a sprite without needing to grab screen size
        // TODO: Proper initialization
        float mOffsetX = 640.f;
        float mOffsetY = 360.f;
    };

    // The camera class handles positioning the virtual viewport in the game
    //  - The x/y position represent the center of the camera viewport
    class CameraService : public Service {
    public:
        CameraService();
        ~CameraService();

        const CameraInfo& GetCameraInfo() const;
        void SetCameraPosition(float x, float y);

    private:
        CameraInfo mCameraInfo;
    };

}