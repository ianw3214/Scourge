#pragma once

#include <string>

namespace Shade {

    class Service {
    public:
        Service(const std::string& name);
        virtual ~Service();

        const std::string& GetName() const;
    private:
        std::string mName;
    };

}