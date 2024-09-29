#pragma once

#include "thirdparty/json_fwd.h"
#include "serialization/valueserialization_impl.h"

template <typename Type>
void SerializeValue(const Type& value_, const char* name_, nlohmann::json& data_)
{
    Serializer<Type>::Serialize(value_, name_, data_);
}

template <typename Type>
void DeserializeValue(Type& outValue_, const char* name_, const nlohmann::json& data_)
{
    Serializer<Type>::Deserialize(outValue_, name_, data_);
}
