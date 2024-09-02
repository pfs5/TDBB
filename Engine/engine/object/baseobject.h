#pragma once
#include <type_traits>
#include <string>
#include <vector>
#include <cstdint>
#include <functional>

#include "serialization/serializable.h"
#include "util/enumutil.h"
#include "util/hash.h"
#include "util/typeid.h"

#define IMPLEMENT_BASE_OBJECT(Type_) \
    BaseObjectDefinition<Type_> g_##BaseObjectDefinition_##Type_;

MAKE_ENUM(
    EObjectCategory, uint8_t
    , BaseObject
    , Entity
    , EntityComponent
    );

class BaseObject : public ISerializable
{
public:
    virtual void Serialize(const nlohmann::json& data_) const override {}
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override { return true; }
    
protected:
    BaseObject() = default;
};

class Class
{
public:
    using Instantiator = std::function<BaseObject*()>;
    
public:
    Class(const char* name_, const Instantiator& instantiator_):
        _name{ name_ },
        _nameHash{ HashString(name_) },
        _instantiator{ instantiator_ }
    {
        
    }

    BaseObject* InstantiateObject() const
    {
        return _instantiator();
    }
    
    const std::string& GetName() const { return _name; }
    HashType GetNameHash() const { return _nameHash; }
    
private:
    std::string _name;
    HashType _nameHash;
    Instantiator _instantiator;
};

// ptodo - temp
struct ClassCollection
{
    ClassCollection()
    {
    }

    void Add(const Class& c_, EObjectCategory category_)
    {
        // ensure(category_ != EObjectCategory::Count);
        
        _data[static_cast<std::underlying_type_t<EObjectCategory>>(category_)].emplace_back(c_);
    }

    const std::vector<Class>& Get(EObjectCategory category_) const
    {
        return _data[static_cast<std::underlying_type_t<EObjectCategory>>(category_)];
    }

    std::vector<Class>& Get(EObjectCategory category_)
    {
        return const_cast<std::vector<Class>&>(const_cast<const ClassCollection&>(*this).Get(category_));   
    }
    
    const Class* FindClass(const char* name) const
    {
        const HashType nameHash = HashString(name);
        for (EObjectCategory category : EObjectCategoryIterable{})
        {
            for (const Class& c : Get(category))
            {
                if (c.GetNameHash() == nameHash)
                {
                    return &c;
                }
            }
        }

        return nullptr;   
    }

private:
    std::vector<Class> _data [static_cast<std::underlying_type_t<EObjectCategory>>(EObjectCategory::Count)];
};

class ObjectRepository
{
    friend struct BaseObjectDefinitionBase;

public:
    static const std::vector<Class>& GetClasses(EObjectCategory category_) { return _collection.Get(category_); }
    static const Class* FindClass(const char* name_) { return _collection.FindClass(name_); }
    
private:
    inline static ClassCollection _collection;

private:
    static void AddClass(const Class& class_, EObjectCategory category_) { _collection.Add(class_, category_); }
};

struct BaseObjectDefinitionBase
{
protected:
    BaseObjectDefinitionBase(EObjectCategory category_):
        _category {category_}
    {
        
    }
    
protected:
    void AddClass(const Class& class_) const
    {
        ObjectRepository::AddClass(class_, _category);
    }

private:
    EObjectCategory _category;
};

template <typename ObjectType, EObjectCategory Category = EObjectCategory::BaseObject>
struct BaseObjectDefinition : public BaseObjectDefinitionBase
{
    BaseObjectDefinition():
        BaseObjectDefinitionBase(Category)
    {
        static_assert(std::is_base_of_v<BaseObject, ObjectType> && "T must inherit BaseObject.");

        // ptodo - map with add unique

        const std::string typeName = TypeId<ObjectType>::GetName();
        AddClass({ typeName.c_str(),
            []() -> BaseObject*
            {
                // ptodo - memory management for objects
                return new ObjectType();
            }});
    }
};


