#include "animatedSpriteComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/graphics/command/setColourMultiplier.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"

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
            // If there is a transition, then go to that state
            if (StateInfo.mTransition.empty())
            {
                mCurrentFrame = StateInfo.mStartFrame;
            }
            else
            {
                ChangeAnimationState(StateInfo.mTransition);
            }
        }
        // If the current frame has an event, run the callback
        auto it = mEvents.find(mCurrentFrame);
        if (it != mEvents.end())
        {
            it->second(mEntityRef);
        }
    }
}

// ======================================
void Shade::AnimatedSpriteComponent::AddRenderCommands(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue)
{
    bool useMultiplier = false;
    if (mColourMultiplier.r != 1.f || mColourMultiplier.g != 1.f || mColourMultiplier.b != 1.f)
    {
        useMultiplier = true;
        commandQueue.emplace_back(std::make_unique<SetColourMultiplierCommand>(mColourMultiplier));
    }

    textureSourceInfo SourceInfo;
    const float FrameWidth = 1.0 / mTileSheetInfo.mColumns;
    const float FrameHeight = 1.0 / mTileSheetInfo.mRows;
    SourceInfo.w = FrameWidth;
    SourceInfo.h = FrameHeight;
    SourceInfo.x = FrameWidth * static_cast<float>(mCurrentFrame % mTileSheetInfo.mColumns);
    SourceInfo.y = FrameHeight * static_cast<float>(mCurrentFrame / mTileSheetInfo.mColumns);
    
    const float DrawX = GetRenderX();
    const float DrawY = GetRenderY();
    commandQueue.emplace_back(std::make_unique<DrawTextureCommand>(DrawX, DrawY, mRenderWidth, mRenderHeight, mTextureHandle, false, SourceInfo, mRenderLayer));

    if (useMultiplier)
    {
        commandQueue.emplace_back(std::make_unique<ResetColourMultiplierCommand>());
    }
}

// ======================================
void Shade::AnimatedSpriteComponent::ChangeAnimationState(const std::string& newState, bool restartSameAnimation)
{
    if (mStates.find(mCurrentState) == mStates.end())
    {
        LogService* logService = ServiceProvider::GetCurrentProvider()->GetService<LogService>();
        logService->LogError("Tried to change to non-existent animation state: " + newState);
        return;
    }
    if (mCurrentState == newState && !restartSameAnimation)
    {
        return;
    }
    mCurrentState = newState;
    mCurrentFrame = mStates[mCurrentState].mStartFrame;
    mElapsedTime = 0.f;
}