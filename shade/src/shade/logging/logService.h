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

        void LogInfo(const std::string& infoMessage);
        void LogWarning(const std::string& warningMessage);
        void LogError(const std::string& errorMessage);

        void RegisterLogger(std::unique_ptr<Logger>&& logger);
    private:
        std::vector<std::unique_ptr<Logger>> mLoggers;
    };

}