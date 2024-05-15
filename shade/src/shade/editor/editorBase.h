#pragma once

#include <string>

namespace Shade {

    class EditorBase {
    public:
        EditorBase(const std::string& name);
        virtual ~EditorBase();

        const std::string& GetName() const;
    private:
        std::string mName;
    };

}