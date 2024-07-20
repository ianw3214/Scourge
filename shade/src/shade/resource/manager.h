#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "shade/instance/service/service.h"
#include "shade/resource/handle.h"

namespace Shade {

    class Resource;

    // TODO: Ability to load a resource and then immediately release it
    //  - Alternatively, load a resource but let caller take ownership
    class ResourceManager : public Service {
    public:
        ResourceManager();
        ~ResourceManager();

        template<class T>
        ResourceHandle LoadResource(const std::string& path);

        template<class T>
        T* GetResource(ResourceHandle handle) const;

    private:
        // Map of path string to handle for easy lookup without searching through all resources
        //  - Important to keep this in sync with the actual resources array
        std::unordered_map<std::string, ResourceHandle> mHandleMap;
        std::vector<std::unique_ptr<Resource>> mResources;
    };

}

#include "shade/resource/resource.h"

namespace Shade {

    template<class T>
    ResourceHandle ResourceManager::LoadResource(const std::string& path)
    {
        auto it = mHandleMap.find(path);
        if (it == mHandleMap.end())
        {
            const size_t index = mResources.size();
            Resource* loadedResource = T::Load(path);
            if (loadedResource == nullptr)
            {
                return ResourceHandle::Invalid;
            }
            mResources.emplace_back(loadedResource);
            const uint32_t id = mResources.back()->GetResourceID();
            ResourceHandle Handle(index, id);
            mHandleMap.emplace(path, Handle);
            return Handle;
        }
        return it->second;
    }

    template<class T>
    T* ResourceManager::GetResource(ResourceHandle handle) const
    {
        // TODO: Warnings/errors can be added here for easier debugging
        if (handle.GetIndex() >= mResources.size())
        {
            return nullptr;
        }
        Resource* Result = mResources[handle.GetIndex()].get();
        if (Result->GetResourceID() != handle.GetID())
        {
            return nullptr;
        }
        return dynamic_cast<T*>(Result);
    }

}