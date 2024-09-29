#include "valueserialization_impl.h"

#include "engine/object/baseobject.h"
#include "serialization/serialization.h"
#include "thirdparty/json.h"

#define IMPLEMENT_BASIC_SERIALIZER(type_) \
    template<> \
    void Serializer<type_>::Serialize(TypeConstRef value_, const char* name_, nlohmann::json& data_) \
    { \
        data_[name_] = value_; \
    } \
    template<> \
    void Serializer<type_>::Deserialize(TypeRef outValue_, const char* name_, const nlohmann::json& data_) \
    { \
        if (data_.contains(name_)) { data_[name_].template get_to<type_>(outValue_); } \
    } \

// Implementations
IMPLEMENT_BASIC_SERIALIZER(bool);

IMPLEMENT_BASIC_SERIALIZER(uint8_t);
IMPLEMENT_BASIC_SERIALIZER(uint16_t);
IMPLEMENT_BASIC_SERIALIZER(uint32_t);
IMPLEMENT_BASIC_SERIALIZER(uint64_t);

IMPLEMENT_BASIC_SERIALIZER(int8_t);
IMPLEMENT_BASIC_SERIALIZER(int16_t);
IMPLEMENT_BASIC_SERIALIZER(int32_t);
IMPLEMENT_BASIC_SERIALIZER(int64_t);

IMPLEMENT_BASIC_SERIALIZER(float);

IMPLEMENT_BASIC_SERIALIZER(sf::Vector2f);
IMPLEMENT_BASIC_SERIALIZER(std::string);

template<>
void Serializer<const Class*>::Serialize(TypeConstRef value_, const char* name_, nlohmann::json& data_)
{
    data_[name_] = value_->GetName();
}
template<>
void Serializer<const Class*>::Deserialize(TypeRef outValue_, const char* name_, const nlohmann::json& data_)
{
    const std::string className = data_[name_].get<std::string>();
    const Class* const classPtr = ObjectRepository::FindClass(className.c_str());
    ensure(classPtr != nullptr);

    outValue_ = classPtr;
}

#undef IMPLEMENT_BASIC_SERIALIZER