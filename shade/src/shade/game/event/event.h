#pragma once

namespace Shade {

    struct BooleanGameplayEvent {
        bool mTriggered = false;
        bool mReleased = false;
        bool mHeld = false;
    };

}