#pragma once

#include "errorhandling/assert.h"

// Helper classes for easier enum usage.
template <typename EnumType>
struct EnumIterator
{
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type = EnumType;
    using enum_underlying_type = std::underlying_type_t<EnumType>;    

    EnumIterator(EnumType value_):
        _value { static_cast<enum_underlying_type>(value_) }
    {
            
    }

    value_type operator*() const { return static_cast<value_type>( _value ); }
    EnumIterator& operator++() { ++_value; return *this; }
    EnumIterator& operator++(int) { EnumIterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const EnumIterator& lhs, const EnumIterator& rhs) { return lhs._value == rhs._value; }
    friend bool operator!= (const EnumIterator& lhs, const EnumIterator& rhs) { return !(lhs == rhs); }

private:
    enum_underlying_type _value = 0;
};

template <typename EnumType>
struct IterableEnum
{
    EnumIterator<EnumType> begin() { return { static_cast<EnumType>(std::underlying_type_t<EnumType>{}) }; }
    EnumIterator<EnumType> end() { return { EnumType::Count }; }
};

// Defining enums to include ToString.
// A bit of macro magic but it works.

#define PARENS ()

// Rescan macro tokens 256 times
#define EXPAND(arg_) EXPAND1(EXPAND1(EXPAND1(EXPAND1(arg_))))
#define EXPAND1(arg_) EXPAND2(EXPAND2(EXPAND2(EXPAND2(arg_))))
#define EXPAND2(arg_) EXPAND3(EXPAND3(EXPAND3(EXPAND3(arg_))))
#define EXPAND3(arg_) EXPAND4(EXPAND4(EXPAND4(EXPAND4(arg_))))
#define EXPAND4(arg_) arg_

// Foreach
#define FOR_EACH(macro_, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro_, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro_, a1_, ...)                   \
    macro_(a1_)                                             \
    __VA_OPT__(FOR_EACH_AGAIN PARENS (macro_, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define ENUM_CASE(name_) case name_: return #name_;

/**
 * Implements an enum with some convenient utilities.
 *
 * Example: MAKE_ENUM(EMyEnum, uint8_t, Value1, Value2);
 * 1/ Creates an enum class EMyEnum : uint8_t with values Value1 and Value2.
 * 2/ Creates a global ToString function that takes a EMyEnum type and returns a const char*.
 * 3/ Lets you iterate all values of the enum with for (EMyEnum value : EMyEnumIterable{}).
 */
#define MAKE_ENUM(name_, type_, ...)                    \
    enum class name_ : type_ {                          \
        __VA_ARGS__,                                    \
        Count                                           \
    };                                                  \
    constexpr const char* ToString(name_ value_) {     \
        using enum name_;                               \
        switch (value_) {                               \
            FOR_EACH(ENUM_CASE, __VA_ARGS__)            \
            default: return "EnumError"; \
        }                                               \
    }                                                   \
    using name_##Iterable = IterableEnum<name_>;
