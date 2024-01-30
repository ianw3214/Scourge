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

// ======================================
void Shade::CameraService::SetCameraPosition(float x, float y)
{
    mCameraInfo.x = x;
    mCameraInfo.y = y;
    mCameraInfo.mOffsetX = 640 - x;
    mCameraInfo.mOffsetY = 360 - y;
}