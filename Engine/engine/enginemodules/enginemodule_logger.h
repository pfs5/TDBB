#pragma once
#include "engine/engineaccess.h"
#include "engine/enginemodule.h"

// ptodo - simple logger for now
class EngineModule_Logger : public EngineModule<EngineModule_Logger>, public IEngineLogAccess
{
    using Super = EngineModule<EngineModule_Logger>;

public:
    using Super::Super;

    // EngineModule
    virtual void Init() final;
    virtual void Shutdown() final;
    
    // IEngineLogAccess
    virtual void Log(const char* string_) final;
    virtual void LogKey(uint32_t key_, const char* string_) final;

    void DrawWindow();

private:
    struct LogEntry
    {
        uint64_t Timestamp;
        std::string String;
    };

private:
    std::vector<LogEntry> _entries;
    std::vector<std::pair<uint32_t, LogEntry>> _entriesPerKey;
};