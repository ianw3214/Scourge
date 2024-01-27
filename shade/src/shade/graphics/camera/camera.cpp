#include "camera.h"

// ======================================
Shade::CameraService::CameraService()
    : Service("CameraService")
{

}
// ======================================
Shade::CameraService::~CameraService()
{

}

// ======================================
const Shade::CameraInfo& Shade::CameraService::GetCameraInfo() const
{
    return mCameraInfo;
}