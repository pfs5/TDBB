#pragma once

#include "engine/entity/propertyserialization.h"
#include "thirdparty/json.h"
#include "util/stringutil.h"
#include "util/typeutils.h"

#include <vector>

class PropertyBase
{
public:
    // Container used for basic reflection.
    PropertyBase(std::vector<PropertyBase*>& container_, StringLiteral name_):
        _name{ name_ }
    {
        container_.emplace_back(this);
    }

    const char* GetName() const { return *_name; }
    
    virtual void Serialize(nlohmann::json& data_) const = 0;
    virtual void Deserialize(const nlohmann::json& data_) = 0;
    virtual void DrawEditor();

    virtual std::string ValueToString() const = 0;

private:
    StringLiteral _name;
};

template <typename Type>
class Property : public PropertyBase
{
    using Super = PropertyBase;

public:
    Property(std::vector<PropertyBase*>& container_, StringLiteral name_, const Type& defaultValue_ = {}):
        Super(container_, name_),
        _value{ defaultValue_ }
    {
        
    }

    virtual void Serialize(nlohmann::json& data_) const final { SerializeValue(_value, GetName(), data_); }
    virtual void Deserialize(const nlohmann::json& data_) final { DeserializeValue(_value, GetName(), data_); };
    
    virtual void DrawEditor() final { return Super::DrawEditor(); }

    virtual std::string ValueToString() const final { return ValueToStringImpl(); }

    const Type& operator*() const { return _value; }
    Type& operator*() { return _value; }
    const Type* operator->() const { return &_value; }
    Type* operator->() { return &_value; }

    bool operator==(const Property& other_) const { return _value == other_._value; }
    bool operator!=(const Property& other_) const { return !operator==(other_); }

    Property& operator=(const Type& value_) { _value = value_; return *this; }
    operator const Type&() const { return _value; }
    operator Type&() { return _value; }
    
    // Disable copy
    Property(const Property& other_) = delete;
    Property& operator=(const Property& other_) = delete;

    // Disable move
    Property(Property&& other_) = delete;
    Property& operator=(Property&& other_) = delete;
    
private:
    Type _value;

private:
    template<typename T = Type, class Enable = void>
    std::string ValueToStringImpl() const { return "[missing to-string]"; }

    template<typename T = Type, typename std::enable_if_t<HasToString<T>::value>>
    std::string ValueToStringImpl() const { return std::to_string(_value); }
};

template <typename Type>
using EntityProperty = Property<Type>; 
    
// ptodo - move this somewhere
template<> void Property<bool>::DrawEditor();

template<> void Property<uint8_t>::DrawEditor();
template<> void Property<uint16_t>::DrawEditor();
template<> void Property<uint32_t>::DrawEditor();
template<> void Property<uint64_t>::DrawEditor();
template<> void Property<int8_t>::DrawEditor();
template<> void Property<int16_t>::DrawEditor();
template<> void Property<int32_t>::DrawEditor();
template<> void Property<int64_t>::DrawEditor();
template<> void Property<float>::DrawEditor();

template<> void Property<sf::Vector2f>::DrawEditor();
template<> void Property<std::string>::DrawEditor();
template<> void Property<const Class*>::DrawEditor();