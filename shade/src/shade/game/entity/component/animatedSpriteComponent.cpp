#include "animatedSpriteComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(float renderWidth, float renderHeight, std::string texturePath, int renderLayer, RenderAnchor renderAnchor)
    : SpriteComponent(renderWidth, renderHeight, texturePath, renderLayer, renderAnchor)
{

}

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(float renderWidth, float renderHeight, std::string texturePath, TilesheetInfo tileInfo, std::unordered_map<std::string, AnimationStateInfo> states, const std::string& initialState, int renderLayer, RenderAnchor renderAnchor)
    : SpriteComponent(renderWidth, renderHeight, texturePath, renderLayer, renderAnchor)
    , mTileSheetInfo(tileInfo)
    , mStates(states)
    , mCurrentState(initialState)
{
    // TODO: Crash if the current state isn't an actual valid state
    // Update current frame based on the initial state
    mCurrentFrame = mStates[mCurrentState].mStartFrame;
}

// ======================================
void Shade::AnimatedSpriteComponent::Update(float deltaSeconds) 
{
    mElapsedTime += deltaSeconds;
    constexpr float frameTime = 1.f / 12.f;
    if (mElapsedTime > frameTime)
    {
        mElapsedTime = 0.f;
        // TODO: Crash if the current state isn't an actual valid state
        AnimationStateInfo StateInfo = mStates[mCurrentState];
        if (++mCurrentFrame > StateInfo.mEndFrame) 
        {
            mCurrentFrame = StateInfo.mStartFrame;
        }
    }
}

// ======================================
std::unique_ptr<Shade::DrawTextureCommand> Shade::AnimatedSpriteComponent::CreateRenderCommand()
{
    textureSourceInfo SourceInfo;
    const float FrameWidth = 1.0 / mTileSheetInfo.mColumns;
    const float FrameHeight = 1.0 / mTileSheetInfo.mRows;
    SourceInfo.w = FrameWidth;
    SourceInfo.h = FrameHeight;
    SourceInfo.x = FrameWidth * static_cast<float>(mCurrentFrame % mTileSheetInfo.mColumns);
    SourceInfo.y = FrameHeight * static_cast<float>(mCurrentFrame / mTileSheetInfo.mColumns);
    
    const float DrawX = GetRenderX();
    const float DrawY = GetRenderY();
    return std::make_unique<DrawTextureCommand>(DrawX, DrawY, mRenderWidth, mRenderHeight, mTextureHandle, false, SourceInfo, mRenderLayer);
}

// ======================================
void Shade::AnimatedSpriteComponent::ChangeAnimationState(const std::string& newState)
{
    mCurrentState = newState;
    mCurrentFrame = mStates[mCurrentState].mStartFrame;
    mElapsedTime = 0.f;
}