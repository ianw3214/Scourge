#pragma once
#ifdef DEBUG_BREACH

#include "shade/module/module.h"
#include "shade/common/colour.h"
#include "shade/common/vec.h"

#include <vector>

// ======================================
class CustomDebugModule : public Shade::Module
{
public:
    // virtual void Update(float deltaSeconds) override;
    virtual void Render(std::vector<std::unique_ptr<Shade::RenderCommand>>& commandQueue) override;

    struct DebugLine {
        Shade::Vec2 mPos;
        float mHeight = 0.f;
    };
    struct DebugRect {
        Shade::Vec2 mPos;
        float mWidth = 10.f;
        float mHeight = 10.f;
        Shade::Colour mColour;
    };
    // TODO: Consider the ordering here if that ever becomes relevant
    static std::vector<DebugLine> mDebugLines;
    static std::vector<DebugRect> mDebugRects;

};

#endif