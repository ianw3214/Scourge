#pragma once

#include <memory>
#include <vector>

namespace Shade {

    class Service;

    // A Service provider handles ownership of services and acts as a central place
    //  to access the services in a game
    // TODO: Figure out more efficient way to map type to service
    class ServiceProvider {
    public:
        ServiceProvider();
        ~ServiceProvider();

        bool RegisterService(Service* NewService);
        template<class T>
        T* GetService() const;

        // Using statics for now as easy solution
        //  - If things start getting multithreaded, may need a more elegant solution
        static void RegisterProvider(ServiceProvider* Provider);
        static ServiceProvider* GetCurrentProvider();
        static inline ServiceProvider* sProvider = nullptr;
    private:
        std::vector<std::unique_ptr<Service>> mServices;
    };

}

namespace Shade {

    template<class T>
    T* ServiceProvider::GetService() const 
    {
        for (const std::unique_ptr<Service>& ServiceIt : mServices)
        {
            T* DerivedService = dynamic_cast<T*>(ServiceIt.get());
            if (DerivedService != nullptr)
            {
                return DerivedService;
            }
        }
        return nullptr;
    }

}