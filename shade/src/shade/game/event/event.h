#pragma once

namespace Shade {

    struct BooleanGameplayEvent {
        bool mTriggered = false;
        bool mReleased = false;
        bool mHeld = false;
    };

    struct IntGameplayEvent {
        int mValue = 0;
    };

    struct FloatGameplayEvent {
        float mValue = 0.f;
    };

}