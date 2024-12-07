#pragma once

#include "engine/engineaccess.h"
#include "util/stringutil.h"

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

template <typename ... Args>
void Log(const char* format_, Args... args_)
{
    const std::string str = StringFormat(format_, std::forward<Args>(args_)...);
    Log(str.c_str());
}

template <typename ... Args>
void LogKey(uint32_t key_, const char* format_, Args... args_)
{
    const std::string str = StringFormat(format_, std::forward<Args>(args_)...);
    LogKey(key_, str.c_str());
}