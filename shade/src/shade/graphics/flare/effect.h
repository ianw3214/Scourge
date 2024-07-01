#pragma once

#include "shade/common/vec.h"
#include "shade/resource/handle.h"

namespace Shade {

    // Base effect that just renders a sprite for a certain lifetime
    // TODO: Can probably save the effect definition and state separately
    //  - That way, different effects can share the same definition
    //  - instead of having to re-define the effect every time
    class Effect {
    public:
        // Effect definitions
        Shade::Vec2 mPosition;
        float mWidth = 10.f;
        float mHeight = 10.f;
        ResourceHandle mTextureHandle = ResourceHandle::Invalid;
        int mRenderLayer = 0;

        // Effect "state"
        float mLifetime = 0.f;
    };

}