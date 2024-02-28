#include "logService.h"

#include "shade/logging/logger.h"

// ======================================
Shade::LogService::LogService()
    : Service("LogService")
{

}

// ======================================
Shade::LogService::~LogService() = default;

// ======================================
void Shade::LogService::LogInfo(const std::string& infoMessage)
{
    for (std::unique_ptr<Logger>& CurrLogger : mLoggers)
    {
        CurrLogger->LogInfo(infoMessage);
    }
    mLogs.emplace_back(infoMessage);
}

// ======================================
void Shade::LogService::LogWarning(const std::string& warningMessage)
{
    for (std::unique_ptr<Logger>& CurrLogger : mLoggers)
    {
        CurrLogger->LogWarning(warningMessage);
    }
    mLogs.emplace_back(warningMessage);
}

// ======================================
void Shade::LogService::LogError(const std::string& errorMessage)
{
    for (std::unique_ptr<Logger>& CurrLogger : mLoggers)
    {
        CurrLogger->LogError(errorMessage);
    }
    mLogs.emplace_back(errorMessage);
}

// ======================================
void Shade::LogService::RegisterLogger(std::unique_ptr<Logger>&& logger)
{
    mLoggers.emplace_back(std::move(logger));
}

// ======================================
const std::vector<std::string>& Shade::LogService::getLogs() const
{
    return mLogs;
}