#pragma once

#include <vector>
#include <memory>

#include "shade/module/module.h"
#include "shade/game/event/mapping.h"
#include "shade/game/event/source.h"
#include "shade/game/entity/container.h"

namespace Shade {

    class Entity;
    class GameWorldModule : public Module, public GameplayEventSource, public EntityContainer {
    public:
        GameWorldModule();
        ~GameWorldModule();

        virtual void Update(float deltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& commandQueue) override;
        virtual bool HandleEvent(const InputEvent& event) override;

    protected:
        InputMapping mInputMapping;
    };

}