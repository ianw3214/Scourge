#pragma once

#include <memory>
#include <vector>

#include "shade/instance/service/service.h"

namespace Shade {

    class Logger;

    class LogService : public Service {
    public:
        LogService();
        ~LogService();

        void LogInfo(const std::string& InfoMessage);
        void LogWarning(const std::string& WarningMessage);
        void LogError(const std::string& ErrorMessage);

        void RegisterLogger(std::unique_ptr<Logger>&& NewLogger);
    private:
        std::vector<std::unique_ptr<Logger>> mLoggers;
    };

}