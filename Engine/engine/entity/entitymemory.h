#pragma once

class EntityBase;

namespace EntityMemory
{
    template <typename EntityType>
    EntityType* InstantiateEntityObject()
    {
        // static_assert(std::is_base_of_v<EntityBase, EntityType>);
        static_assert(true);
        
        // ptodo - memory handling here
        
        return new EntityType();
    }
}
