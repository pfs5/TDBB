#pragma once

#include "engine/entity/propertyserialization.h"
#include "thirdparty/json.h"
#include "util/stringutil.h"
#include "util/typeutils.h"

#include <vector>

class EntityComponentBase;
class EntityBase;
class IPropertyContainer;

class PropertyBase
{
public:
    // Container used for basic reflection.
    PropertyBase(IPropertyContainer* parent_, std::vector<PropertyBase*>& container_, StringLiteral name_):
        _name{ name_ },
        _parentContainer{ parent_ }
    {
        container_.emplace_back(this);
    }

    const char* GetName() const { return *_name; }
    
    virtual void Serialize(nlohmann::json& data_) const = 0;
    virtual void Deserialize(const nlohmann::json& data_) = 0;
    virtual void DrawEditor();

    virtual std::string ValueToString() const = 0;

protected:
    virtual void OnValueChanged();
    
private:
    StringLiteral _name;
    IPropertyContainer* _parentContainer;
};

template <typename Type>
class Property : public PropertyBase
{
    using Super = PropertyBase;
    
public:
    Property(IPropertyContainer* parent_, std::vector<PropertyBase*>& container_, StringLiteral name_, const Type& defaultValue_ = {}):
        Super(parent_, container_, name_),
        _value{ defaultValue_ }
    {
        
    }

    virtual void Serialize(nlohmann::json& data_) const final { SerializeValue(_value, GetName(), data_); }
    virtual void Deserialize(const nlohmann::json& data_) final { DeserializeValue(_value, GetName(), data_); };
    
    virtual void DrawEditor() final { return Super::DrawEditor(); }
    virtual std::string ValueToString() const final { return ValueToStringImpl(); }

    void Set(const Type& value_) { _value = value_; OnValueChanged(); }
    
    // Value assignment only via operator= or Set
    const Type& operator*() const { return _value; }
    // Type& operator*() { return _value; }
    const Type* operator->() const { return &_value; }
    Type* operator->() { return &_value; }

    bool operator==(const Property& other_) const { return _value == other_._value; }
    bool operator!=(const Property& other_) const { return !operator==(other_); }

    Property& operator=(const Type& value_) { Set(value_); return *this; }
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

template <typename Type, typename EntityClass>
class EntityProperty : public Property<Type>
{
    using Super = Property<Type>;

    template<typename ObjectType>
    using CallbackFunctionPointer = void (ObjectType::*)();
    
public:
    EntityProperty(EntityClass& outer_, std::vector<PropertyBase*>& container_, StringLiteral name_,
        const Type& defaultValue_ = {}, CallbackFunctionPointer<EntityClass> onChangeDelegate_ = nullptr):
        Super(&outer_, container_, name_, defaultValue_),
        _outer{ &outer_ },
        _onChangeDelegate{ onChangeDelegate_ }
    {
        static_assert(std::is_base_of_v<EntityBase, EntityClass>);
    }

    EntityProperty& operator=(const Type& value_) { Super::operator=(value_); return *this; }

private:
    EntityClass* _outer = nullptr;
    CallbackFunctionPointer<EntityClass> _onChangeDelegate = nullptr;
};

template <typename Type, typename ComponentClass>
class ComponentProperty : public Property<Type>
{
    using Super = Property<Type>;

    template<typename ObjectType>
    using CallbackFunctionPointer = void (ObjectType::*)();
    
public:
    ComponentProperty(ComponentClass& outer_, std::vector<PropertyBase*>& container_, StringLiteral name_,
        const Type& defaultValue_ = {}, CallbackFunctionPointer<ComponentClass> onChangeDelegate_ = nullptr):
        Super(&outer_, container_, name_, defaultValue_),
        _outer{ &outer_ },
        _onChangeDelegate{ onChangeDelegate_ }
    {
        static_assert(std::is_base_of_v<EntityComponentBase, ComponentClass>);
    }

    ComponentProperty& operator=(const Type& value_) { Super::operator=(value_); return *this; }

private:
    ComponentClass* _outer;
    CallbackFunctionPointer<ComponentClass> _onChangeDelegate;
};

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