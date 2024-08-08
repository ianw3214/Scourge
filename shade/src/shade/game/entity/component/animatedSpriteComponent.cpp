#include "animatedSpriteComponent.h"

#include <cassert>

#include "shade/game/entity/entity.h"
#include "shade/graphics/command/drawTexture.h"
#include "shade/graphics/command/setColourMultiplier.h"
#include "shade/instance/service/provider.h"
#include "shade/logging/logService.h"
#include "shade/resource/manager.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#ifdef BUILD_SHADE_EDITOR
// ======================================
// TODO: Consider using `ImGuiInputTextFlags_EnterReturnsTrue` to lighten load
void Shade::AnimatedSpriteComponent::ShowImguiDetails()
{
    SpriteComponent::ShowImguiDetails();

    const bool initialStateChanged = ImGui::InputText("initial state", &mInitialState);
    const bool frameDataChanged = ImGui::InputText("frame data path", &mFrameDataPath);
    if (initialStateChanged)
    {
        if (mStates.find(mInitialState) != mStates.end())
        {
            ChangeAnimationState(mInitialState);
        }
    }
    if (frameDataChanged)
    {
        // TODO: Refresh all associated frame data
    }

    if (ImGui::TreeNode("Transitions"))
    {
        // TODO: Maybe want to implement deleting transitions, maybe not important?
        bool transitionTableDirty = false;
        if (ImGui::Button("Add Transition"))
        {
            mTransitionTable.emplace_back();
            transitionTableDirty = true;
        }
        for (int n = 0; n < mTransitionTable.size(); n++)
        {
            if (ImGui::TreeNode(std::to_string(n).c_str()))
            {
                transitionTableDirty |= ImGui::InputText("from", &mTransitionTable[n].first);
                transitionTableDirty |= ImGui::InputText("to", &mTransitionTable[n].second);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();

        if (transitionTableDirty)
        {
            // Clear all previous transitions before reloading all new ones again
            for (auto& pair : mStates)
            {
                pair.second.mTransition.clear();
            }
            for (const auto& transition : mTransitionTable)
            {
                mStates[transition.first].mTransition = transition.second;
            }
        }
    }
}
#endif

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, int renderLayer, RenderAnchor renderAnchor)
    : SpriteComponent(renderWidth, renderHeight, texturePath, renderLayer, renderAnchor)
{

}

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, TilesheetInfo tileInfo, std::unordered_map<std::string, AnimationStateInfo> states, const std::string& initialState, int renderLayer, RenderAnchor renderAnchor)
    : SpriteComponent(renderWidth, renderHeight, texturePath, renderLayer, renderAnchor)
    , mTileSheetInfo(tileInfo)
    , mStates(states)
    , mCurrentState(initialState)
{
    assert(mStates.find(mCurrentState) != mStates.end() && "Current state needs to be a valid state");

    // Update current frame based on the initial state
    mCurrentFrame = mStates[mCurrentState].mStartFrame;

#ifdef BUILD_SHADE_EDITOR
    mInitialState = initialState;
#endif
}

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, const AnimationFrameData* frameData, const std::string& initialState, int renderLayer, RenderAnchor renderAnchor)
    : SpriteComponent(renderWidth, renderHeight, texturePath, renderLayer, renderAnchor)
    , mCurrentState(initialState)
{
    assert(frameData != nullptr && "Null frame data passed in to initialize animated sprite component");

    mTileSheetInfo = TilesheetInfo{ frameData->GetFrameWidth(), frameData->GetFrameHeight(), frameData->GetColumns(), frameData->GetRows() };
    for (const AnimationFrameInfo& frame : frameData->GetAnimationFrames())
    {
        mStates[frame.mName] = AnimationStateInfo{ frame.mStart, frame.mEnd };
    }
    assert(mStates.find(mCurrentState) != mStates.end() && "Current state needs to be a valid state");

    mCurrentFrame = mStates[mCurrentState].mStartFrame;

#ifdef BUILD_SHADE_EDITOR
    mInitialState = initialState;
#endif
}

// ======================================
Shade::AnimatedSpriteComponent::AnimatedSpriteComponent(float renderWidth, float renderHeight, const std::string& texturePath, const std::string& frameDataPath, TransitionTable transitionTable, const std::string& initialState, int renderLayer, RenderAnchor renderAnchor)
    : SpriteComponent(renderWidth, renderHeight, texturePath, renderLayer, renderAnchor)
    , mCurrentState(initialState)
{
    Shade::ResourceManager* resourceManager = Shade::ServiceProvider::GetCurrentProvider()->GetService<Shade::ResourceManager>();
    Shade::ResourceHandle frameDataHandle = resourceManager->LoadResource<Shade::AnimationFrameData>(frameDataPath);
    assert(frameDataHandle.IsValid() && "Frame data must be valid to instantiate animated sprite");
    
    Shade::AnimationFrameData* frameData = resourceManager->GetResource<Shade::AnimationFrameData>(frameDataHandle);

    mTileSheetInfo = TilesheetInfo{ frameData->GetFrameWidth(), frameData->GetFrameHeight(), frameData->GetColumns(), frameData->GetRows() };
    for (const AnimationFrameInfo& frame : frameData->GetAnimationFrames())
    {
        mStates[frame.mName] = AnimationStateInfo{ frame.mStart, frame.mEnd };
    }
    assert(mStates.find(mCurrentState) != mStates.end() && "Current state needs to be a valid state");

    for (const auto& transition : transitionTable)
    {
        mStates[transition.first].mTransition = transition.second;
    }

    mCurrentFrame = mStates[mCurrentState].mStartFrame;

#ifdef BUILD_SHADE_EDITOR
    mInitialState = initialState;
    mFrameDataPath = frameDataPath;
    mTransitionTable = transitionTable;
#endif
}

// ======================================
void Shade::AnimatedSpriteComponent::SetAnimationTransition(const std::string& animation, const std::string& transition)
{
    mStates[animation].mTransition = transition;
}

// ======================================
void Shade::AnimatedSpriteComponent::Update(float deltaSeconds) 
{
    mElapsedTime += deltaSeconds;
    constexpr float frameTime = 1.f / 12.f;
    if (mElapsedTime > frameTime)
    {
        mElapsedTime = 0.f;
        
        assert(mStates.find(mCurrentState) != mStates.end() && "Current state needs to be a valid state");
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