#pragma once

#include <string>

namespace Shade {

    class Logger {
    public:
        Logger();
        virtual ~Logger();

        virtual void LogInfo(const std::string& InfoMessage);
        virtual void LogWarning(const std::string& WarningMessage);
        virtual void LogError(const std::string& ErrorMessage);
    };

}