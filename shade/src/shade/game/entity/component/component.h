#pragma once

namespace Shade {

    class Entity;
    class Component {
    public:
        Component(Entity& Owner);
        virtual ~Component();

        virtual void Update(float DeltaSeconds);
    protected:
        Entity& mEntityRef;
    };

}