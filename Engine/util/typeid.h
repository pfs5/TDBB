#pragma once

#include <regex>

template <typename T>
struct TypeId
{
public:
    static std::string GetName()
    {
        // This is a bit hacky but it works.
        // Use regex to find type name from function name
        // TypeHelper<class TypeName>::GetName

        const std::string funName = __FUNCTION__;
        const std::regex pattern {"<class (.*)>"};

        std::smatch regexMatches;
        std::regex_search(funName, regexMatches, pattern);

        const std::string s = regexMatches.str(1);
        return s;
    }
};