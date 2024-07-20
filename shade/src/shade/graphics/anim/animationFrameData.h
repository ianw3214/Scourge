#pragma once

#include "shade/resource/resource.h"

#include <string>
#include <vector>

namespace Shade {

    // ======================================
    struct AnimationFrameInfo {
        std::string mName;
        uint16_t mStart = 0;
        uint16_t mEnd = 0;
    };

    // ======================================
    // AnimationFrameData holds data directly exported from AseSprite JSON files
    class AnimationFrameData : public Shade::Resource {
    public:
        static Shade::Resource* Load(const std::string& path);

    public:
        AnimationFrameData(uint16_t w, uint16_t h, uint16_t rows, uint16_t columns, std::vector<AnimationFrameInfo>&& animationFrames);

        const uint16_t GetFrameWidth() const;
        const uint16_t GetFrameHeight() const;
        const uint16_t GetRows() const;
        const uint16_t GetColumns() const;
        const std::vector<AnimationFrameInfo>& GetAnimationFrames() const;

    private:
        uint16_t mFrameWidth = 0;
        uint16_t mFrameHeight = 0;
        uint16_t mRows = 0;
        uint16_t mColumns = 0;
        std::vector<AnimationFrameInfo> mAnimationFrames;
    };

}