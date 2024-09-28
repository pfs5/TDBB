#pragma once
#include <type_traits>

class BaseObject;

class ObjectCreator
{
    template <typename ObjectType>
    ObjectType* CreateObject()
    {
        static_assert(std::is_base_of_v<BaseObject, ObjectType>);
    
        // ptodo - implement smarter object memory handling here
        return new ObjectType();
    }
};

template <typename ObjectType>
ObjectType* CreateObject()
{
    return ObjectCreator::CreateObject<ObjectType>();
}