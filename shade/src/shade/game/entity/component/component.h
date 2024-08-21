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

        // Initialize is called after ALL of an entities defined components have been created and added
        //  - "Second pass" initialization, used to manage component dependencies on creation
        virtual void Initialize();
        virtual void Update(float deltaSeconds);
    protected:
        // This should be guaranteed to not be null in most use cases
        Entity* mEntityRef = nullptr;

        bool mInitialized = false;
    };

}