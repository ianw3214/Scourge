#pragma once

#include <vector>
#include <memory>

#include "shade/module/module.h"
#include "shade/game/event/mapping.h"
#include "shade/game/event/source.h"

namespace Shade {

    class Entity;
    class GameWorldModule : public Module, public GameplayEventSource {
    public:
        GameWorldModule();
        ~GameWorldModule();

        virtual void Update(float DeltaSeconds) override;
        virtual void Render(std::vector<std::unique_ptr<RenderCommand>>& CommandQueue) override;
        virtual bool HandleEvent(const InputEvent& Event) override;

        void AddEntity(std::unique_ptr<Entity> NewEntity);
    protected:
        std::vector<std::unique_ptr<Entity>> mEntities;

        InputMapping mInputMapping;
    };

}