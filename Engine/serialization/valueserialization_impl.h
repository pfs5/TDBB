#pragma once

#include "valueserialization.h"
#include <SFML/System/Vector2.hpp>

class Class;

#define DECLARE_BASIC_SERIALIZER(type_) \
    template<> \
    void Serializer<type_>::Serialize(TypeConstRef value_, const char* name_, nlohmann::json& data_); \
    template<> \
    void Serializer<type_>::Deserialize(TypeRef outValue_, const char* name_, const nlohmann::json& data_); \

// Define which types we can serialize.
DECLARE_BASIC_SERIALIZER(bool);

DECLARE_BASIC_SERIALIZER(uint8_t);
DECLARE_BASIC_SERIALIZER(uint16_t);
DECLARE_BASIC_SERIALIZER(uint32_t);
DECLARE_BASIC_SERIALIZER(uint64_t);

DECLARE_BASIC_SERIALIZER(int8_t);
DECLARE_BASIC_SERIALIZER(int16_t);
DECLARE_BASIC_SERIALIZER(int32_t);
DECLARE_BASIC_SERIALIZER(int64_t);

DECLARE_BASIC_SERIALIZER(float);

DECLARE_BASIC_SERIALIZER(sf::Vector2f);
DECLARE_BASIC_SERIALIZER(std::string);

template<>
void Serializer<const Class*>::Serialize(TypeConstRef value_, const char* name_, nlohmann::json& data_);
template<>
void Serializer<const Class*>::Deserialize(TypeRef outValue_, const char* name_, const nlohmann::json& data_);

#undef DECLARE_BASIC_SERIALIZER