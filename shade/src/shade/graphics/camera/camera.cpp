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

// ======================================
Shade::Vec2 Shade::CameraService::WorldToScreen(Shade::Vec2 pos) const
{
    return Shade::Vec2{ WorldToScreenX(pos.x), WorldToScreenY(pos.y) };
}

// ======================================
float Shade::CameraService::WorldToScreenX(float x) const
{
    return x + mCameraInfo.mOffsetX;
}

// ======================================
float Shade::CameraService::WorldToScreenY(float y) const
{
    return y + mCameraInfo.mOffsetY;
}

// ======================================
Shade::Vec2 Shade::CameraService::ScreenToWorld(Shade::Vec2 pos) const
{
    return Shade::Vec2{ ScreenToWorldX(pos.x), ScreenToWorldY(pos.y) };
}

// ======================================
float Shade::CameraService::ScreenToWorldX(float x) const
{
    return x - mCameraInfo.mOffsetX;
}

// ======================================
float Shade::CameraService::ScreenToWorldY(float y) const
{
    return y - mCameraInfo.mOffsetY;
}

