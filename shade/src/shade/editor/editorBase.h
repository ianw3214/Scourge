#pragma once

#include <string>

namespace Shade {

    class EditorBase {
    public:
        EditorBase(const std::string& name);
        virtual ~EditorBase();

        const std::string& GetName() const;

        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;
    private:
        std::string mName;
    };

}