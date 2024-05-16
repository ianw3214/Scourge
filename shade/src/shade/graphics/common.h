#pragma once

namespace Shade {
    
    // A set of coordinates and size info that determines the part of a texture that will be drawn
    struct textureSourceInfo {
        float x = 0.0;
        float y = 0.0;
        float w = 1.0;
        float h = 1.0;
    };

    // Defines the point of a texture that counts as its coordinate to render from
    //  - e.g. BOTTOM_LEFT render anchor means the texture will render to the top and right of the desired position
    enum class RenderAnchor {
        BOTTOM_LEFT,
        BOTTOM_MIDDLE,
        MIDDLE
    };

    namespace RenderUtil {

        inline float GetXForRenderAnchor(float x, float width, RenderAnchor anchor)
        {
            switch(anchor) {
            case RenderAnchor::BOTTOM_LEFT: {
                return x;
            } break;
            case RenderAnchor::BOTTOM_MIDDLE:
            case RenderAnchor::MIDDLE: {
                return x - width / 2.0f;
            } break;
            default: {
                return x;
            } break;
        }
        }

        inline float GetYForRenderAnchor(float y, float height, RenderAnchor anchor)
        {
            switch(anchor) {
                case RenderAnchor::BOTTOM_LEFT: 
                case RenderAnchor::BOTTOM_MIDDLE: {
                    return y;
                } break;
                case RenderAnchor::MIDDLE: {
                    return y - height / 2.0f;
                } break;
                default: {
                    return y;
                } break;
            }
        }

    }

}