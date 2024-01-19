#pragma once

#include <unordered_map>

#include "shade/game/entity/component/spriteComponent.h"

namespace Shade {

    struct TilesheetInfo {
        uint16_t mFrameWidth = 0;
        uint16_t mFrameHeight = 0;
        uint16_t mColumns = 0;
        uint16_t mRows = 0;
    };

    struct AnimationStateInfo {
        uint16_t mStartFrame = 0;
        uint16_t mEndFrame = 0;
    };

    class AnimatedSpriteComponent : public SpriteComponent {
    public:
        AnimatedSpriteComponent(Entity& owner);
        AnimatedSpriteComponent(Entity& owner, float renderWidth, float renderHeight, std::string texturePath);
        // TODO: This is a super long constructor
        //  - try to trim it down by separating out the initialization
        AnimatedSpriteComponent(Entity& owner, float renderWidth, float renderHeight, std::string texturePath, TilesheetInfo tileInfo, std::unordered_map<std::string, AnimationStateInfo> states, const std::string& initialState);

        virtual void Update(float deltaSeconds) override;
        virtual std::unique_ptr<DrawTextureCommand> CreateRenderCommand() override;

        void ChangeAnimationState(const std::string& newState);
    public:
        // Animation definition
        TilesheetInfo mTileSheetInfo;
        std::unordered_map<std::string, AnimationStateInfo> mStates;

        // Runtime data
        uint16_t mCurrentFrame = 0;
        float mElapsedTime = 0.f;
        std::string mCurrentState;
    };

}