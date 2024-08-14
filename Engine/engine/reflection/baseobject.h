#pragma once
#include <type_traits>
#include <string>
#include <vector>
#include <cstdint>

#include "errorhandling/assert.h"
#include "util/enumutil.h"
#include "util/typeid.h"

#define IMPLEMENT_BASE_OBJECT(Type_) \
    BaseObjectDefinition<Type_> g_##BaseObjectDefinition_##Type_;

MAKE_ENUM(
    EObjectCategory, uint8_t
    , BaseObject
    , Entity
    , EntityComponent
    );

class BaseObject
{
};

class Class
{
public:
    Class(const char* name_):
        Name{ name_ }
    {
        
    }
    
    // ptodo
    std::string Name;
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

    const auto& Get(EObjectCategory category_)
    {
        return _data[static_cast<std::underlying_type_t<EObjectCategory>>(category_)];
    }

private:
    std::vector<Class> _data [static_cast<std::underlying_type_t<EObjectCategory>>(EObjectCategory::Count)];
};

class ObjectRepository
{
    friend struct BaseObjectDefinitionBase;

public:
    static const std::vector<Class>& GetClasses(EObjectCategory category_) { return _collection.Get(category_); }
    
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
    void AddClass(const Class& class_)
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
        AddClass({typeName.c_str()});
    }
};


