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
void Shade::LogService::LogInfo(const std::string& InfoMessage)
{
    for (std::unique_ptr<Logger>& CurrLogger : mLoggers)
    {
        CurrLogger->LogInfo(InfoMessage);
    }
}

// ======================================
void Shade::LogService::LogWarning(const std::string& WarningMessage)
{
    for (std::unique_ptr<Logger>& CurrLogger : mLoggers)
    {
        CurrLogger->LogWarning(WarningMessage);
    }
}

// ======================================
void Shade::LogService::LogError(const std::string& ErrorMessage)
{
    for (std::unique_ptr<Logger>& CurrLogger : mLoggers)
    {
        CurrLogger->LogError(ErrorMessage);
    }
}

// ======================================
void Shade::LogService::RegisterLogger(std::unique_ptr<Logger>&& NewLogger)
{
    mLoggers.emplace_back(std::move(NewLogger));
}
