#pragma once

#include "shade/common/vec.h"

namespace Shade {

    struct Box {
        // The position represents the bottom left corner of the box
        Vec2 mPosition;
        float mWidth;
        float mHeight;
    };

    inline bool BoxIntersectsBox(const Box& box1, const Box& box2)
    {
        const bool hIntersect = box1.mPosition.x < box2.mPosition.x + box2.mWidth && box1.mPosition.x + box1.mWidth > box2.mPosition.x;
        const bool vIntersect = box1.mPosition.y < box2.mPosition.y + box2.mHeight && box1.mPosition.y + box1.mHeight > box2.mPosition.y;
        return hIntersect && vIntersect;
    }

}