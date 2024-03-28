#pragma once

namespace Shade {

    struct Vec2 {
        float x = 0.f;
        float y = 0.f;

        Vec2() = default;
        Vec2(float x, float y) : x(x), y(y) {}

        inline Vec2 Up(float offset) const {
            return Vec2(x, y + offset);
        }

        inline Vec2 Down(float offset) const {
            return Vec2(x, y - offset);
        }

        inline Vec2 Left(float offset) const {
            return Vec2(x - offset, y);
        }

        inline Vec2 Right(float offset) const {
            return Vec2(x + offset, y);
        }
    };

}