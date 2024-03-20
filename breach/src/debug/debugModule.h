#pragma once
#ifdef DEBUG_BREACH

#include "shade/module/module.h"
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
    static std::vector<DebugLine> mDebugLines;
};

#endif