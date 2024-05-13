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

}