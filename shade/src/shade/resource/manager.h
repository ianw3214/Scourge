#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "shade/instance/service/service.h"
#include "shade/resource/handle.h"

namespace Shade {

    class Resource;

    class ResourceManager : public Service {
    public:
        ResourceManager();
        ~ResourceManager();

        template<class T>
        ResourceHandle LoadResource(const std::string& Path);

        template<class T>
        T* GetResource(ResourceHandle Handle) const;

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
    ResourceHandle ResourceManager::LoadResource(const std::string& Path)
    {
        auto it = mHandleMap.find(Path);
        if (it == mHandleMap.end())
        {
            const size_t Index = mResources.size();
            mResources.emplace_back(T::Load(Path));
            const uint32_t Id = mResources.back()->GetResourceID();
            ResourceHandle Handle(Index, Id);
            mHandleMap.emplace(Path, Handle);
            return Handle;
        }
        return it->second;
    }

    template<class T>
    T* ResourceManager::GetResource(ResourceHandle Handle) const
    {
        // TODO: Warnings/errors can be added here for easier debugging
        if (Handle.GetIndex() >= mResources.size())
        {
            return nullptr;
        }
        Resource* Result = mResources[Handle.GetIndex()].get();
        if (Result->GetResourceID() != Handle.GetID())
        {
            return nullptr;
        }
        return dynamic_cast<T*>(Result);
    }

}