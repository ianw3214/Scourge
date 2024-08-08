#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

#include "shade/game/entity/component/spriteComponent.h"
#include "shade/graphics/anim/animationFrameData.h"

namespace Shade {

    class Entity;

    struct TilesheetInfo {
        uint16_t mFrameWidth = 0;
        uint16_t mFrameHeight = 0;
        uint16_t mColumns = 0;
        uint16_t mRows = 0;
    };

    struct AnimationStateInfo {
        uint16_t mStartFrame = 0;
        uint16_t mEndFrame = 0;
        std::string mTransition = "";   // Empty default transition means the animation state will loop
    };

    // ======================================
    class AnimatedSpriteComponent : public SpriteComponent {
    public:
        typedef std::vector<std::pair<std::string, std::string>> TransitionTable;
#ifdef BUILD_SHADE_EDITOR
    public:
        AnimatedSpriteComponent() {}
        virtual void ShowImguiDetails() override;
    private:
        std::string mInitialState = "";
        std::string mFrameDataPath = "";
        TransitionTable mTransitionTable;
#endif
    public:
        AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE);
        // TODO: This is a super long constructor
        //  - try to trim it down by separating out the initialization
        AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, TilesheetInfo tileInfo, std::unordered_map<std::string, AnimationStateInfo> states, const std::string& initialState, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE);
        AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, const AnimationFrameData* frameData, const std::string& initialState, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE);
        AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, const std::string& frameDataPath, TransitionTable transitionTable, const std::string& initialState, int renderLayer = 0, RenderAnchor renderAnchor = RenderAnchor::MIDDLE);

        virtual void Update(float deltaSeconds) override;
        virtual void AddRenderCommands(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue) override;

        // 'restartSameAnimation' will determine whether the animation should play from the starting frame if the new animation is the same as the existing one
        void ChangeAnimationState(const std::string& newState, bool restartSameAnimation = true);
    public:
        // Animation definition
        TilesheetInfo mTileSheetInfo;
        std::unordered_map<std::string, AnimationStateInfo> mStates;
        std::unordered_map<uint16_t, std::function<void(Entity*)>> mEvents;

        // Runtime data
        uint16_t mCurrentFrame = 0;
        float mElapsedTime = 0.f;
        std::string mCurrentState;
    };

}