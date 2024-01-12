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
        AnimatedSpriteComponent(Entity& Owner);
        AnimatedSpriteComponent(Entity& Owner, float RenderWidth, float RenderHeight, std::string TexturePath);
        // TODO: This is a super long constructor
        //  - try to trim it down by separating out the initialization
        AnimatedSpriteComponent(Entity& Owner, float RenderWidth, float RenderHeight, std::string TexturePath, TilesheetInfo TileInfo, std::unordered_map<std::string, AnimationStateInfo> States, const std::string& InitialState );

        virtual void Update(float DeltaSeconds) override;
        virtual std::unique_ptr<DrawTextureCommand> CreateRenderCommand() override;

        void ChangeAnimationState(const std::string& NewState);
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