#pragma once

namespace Shade {

    class Entity;
    class Component {
#ifdef BUILD_SHADE_EDITOR
    public:
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