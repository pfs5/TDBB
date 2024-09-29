#pragma once

#include "thirdparty/json_fwd.h"

// Base serializer will not compile. Only types with serialization implementation will work.
// See valueserialization_impl.h
template <typename Type>
struct Serializer
{
    using TypeConstRef = std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<Type>>>;
    using TypeRef = std::add_lvalue_reference_t<std::remove_reference_t<Type>>;
    
    // If compilation breaks here, add an implementation for your type in valueserialization_impl. 
    static void Serialize(TypeConstRef value_, const char* name_, nlohmann::json& data_) { typename Type::unimplemented_function; }
    static void Deserialize(TypeRef outValue_, const char* name_, const nlohmann::json& data_) { typename Type::unimplemented_function; }
};
