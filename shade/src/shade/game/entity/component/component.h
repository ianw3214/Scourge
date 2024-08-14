#pragma once

#include <string>

namespace Shade {

    class Entity;
    class Component {
#ifdef BUILD_SHADE_EDITOR
    public:
        virtual const char* GetDisplayName() const { return ""; }
        virtual const char* GetComponentID() const { return ""; }
        virtual void ShowImguiDetails() {}
#endif
    public:
        Component();
        virtual ~Component();

        void SetEntityRef(Entity* entityRef);

        virtual void Update(float deltaSeconds);
    protected:
        // This should be guaranteed to not be null in most use cases
        Entity* mEntityRef = nullptr;
    };

}