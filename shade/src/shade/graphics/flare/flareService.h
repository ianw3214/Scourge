#pragma once

#include "shade/common/vec.h"
#include "shade/graphics/flare/effect.h"
#include "shade/instance/service/service.h"

#include <memory>
#include <vector>

namespace Shade {

    class RenderCommand;

    // ======================================
    // - The Flare vfx/particle service 
    class FlareService : public Service {
    public:
        FlareService();
        ~FlareService();

        void RenderVFX(std::vector<std::unique_ptr<RenderCommand>>& commandQueue, float deltaSeconds);

        void SpawnEffect(Effect effect);

    private:
        // TODO: Is a vector the best data structure for this?
        //  - Maybe an object pool might work well
        std::vector<Effect> mEffects;

    };

}