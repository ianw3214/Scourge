#pragma once

#include <string>

namespace Shade {

    class Entity;
    class KeyValueFile;
    class Component {
#ifdef BUILD_SHADE_EDITOR
    public:
        virtual const char* GetDisplayName() const { return ""; }
        virtual const char* GetComponentID() const { return ""; }
        virtual void ShowImguiDetails() {}
        virtual void SaveToKeyValueFile(Shade::KeyValueFile& file) const {}
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