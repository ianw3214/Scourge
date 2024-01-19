#pragma once

namespace Shade {

    class Entity;
    class Component {
    public:
        Component(Entity& owner);
        virtual ~Component();

        virtual void Update(float deltaSeconds);
    protected:
        Entity& mEntityRef;
    };

}