#pragma once

#include <string_view>

using HashType = size_t;
inline HashType HashString(const char* string_)
{
    return std::hash<std::string_view>{}(string_);
}