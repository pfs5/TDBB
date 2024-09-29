#pragma once

#include "engine/engineaccess.h"

inline void Log(const char* string_)
{
    if (EngineLogAccess::IsValid())
    {
        EngineLogAccess::Get().Log(string_);
    }
}

inline void LogKey(uint32_t key_, const char* string_)
{
    if (EngineLogAccess::IsValid())
    {
        EngineLogAccess::Get().LogKey(key_, string_);
    }
}