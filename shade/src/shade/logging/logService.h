#pragma once

#include <memory>
#include <vector>
#include <string>

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

        const std::vector<std::string>& getLogs() const;
    private:
        std::vector<std::unique_ptr<Logger>> mLoggers;

        // TODO: This should separate messages/warnings/errors
        //  - either by storing in different vectors
        //  - or using a wrapper struct to differentiate <-- I like this better
        std::vector<std::string> mLogs;
    };

}