#include "animatedSpriteComponent.h"

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(Entity& Owner)
    : SpriteComponent(Owner)
{

}

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(Entity& Owner, float RenderWidth, float RenderHeight, std::string TexturePath)
    : SpriteComponent(Owner, RenderWidth, RenderHeight, TexturePath)
{

}

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(Entity& Owner, float RenderWidth, float RenderHeight, std::string TexturePath, TilesheetInfo TileInfo, std::unordered_map<std::string, AnimationStateInfo> States, const std::string& InitialState)
    : SpriteComponent(Owner, RenderWidth, RenderHeight, TexturePath)
    , mTileSheetInfo(TileInfo)
    , mStates(States)
    , mCurrentState(InitialState)
{

}

// ======================================
void Shade::AnimatedSpriteComponent::Update(float DeltaSeconds) 
{
    mElapsedTime += DeltaSeconds;
    constexpr float FrameTime = 1.f / 12.f;
    if (mElapsedTime > FrameTime)
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
    TextureSourceInfo SourceInfo;
    const float FrameWidth = 1.0 / mTileSheetInfo.mColumns;
    const float FrameHeight = 1.0 / mTileSheetInfo.mRows;
    SourceInfo.w = FrameWidth;
    SourceInfo.h = FrameHeight;
    SourceInfo.x = FrameWidth * static_cast<float>(mCurrentFrame % mTileSheetInfo.mColumns);
    SourceInfo.y = FrameHeight * static_cast<float>(mCurrentFrame / mTileSheetInfo.mColumns);
    
    const float DrawX = mEntityRef.GetPositionX();
    const float DrawY = mEntityRef.GetPositionY();
    return std::make_unique<DrawTextureCommand>(DrawX, DrawY, mRenderWidth, mRenderHeight, mTextureHandle, false, SourceInfo);
}