#pragma once

#include <string>

namespace Shade {

    class Logger {
    public:
        Logger();
        virtual ~Logger();

        virtual void LogInfo(const std::string& infoMessage);
        virtual void LogWarning(const std::string& warningMessage);
        virtual void LogError(const std::string& errorMessage);
    };

}