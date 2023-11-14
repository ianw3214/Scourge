#pragma once

#include "shade/module/module.h"

namespace Shade {

    class GameWorldModule : public Module {
    public:
        ~GameWorldModule();

        virtual void Update(float DeltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& CommandQueue) override;
    };

}