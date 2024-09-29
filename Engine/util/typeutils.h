#pragma once
#include <string>
#include <type_traits>

template <typename Type, typename = int>
struct HasToString : std::false_type {};

template <typename Type>
struct HasToString <Type, decltype(std::to_string(Type{}), 0)> : std::true_type {};