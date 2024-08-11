#pragma once

#include <type_traits>

class EntityComponentBase;

// Used to grant access to the entity to certain private parts of the entity component.
// ptodo - should probably be done better with a separate factory instead of entity acting as a factory.
class IEntityComponentAccess
{
    friend class EntityBase;
    
private:
    template <typename ComponentType>
    static ComponentType* CreateComponent()
    {
        static_assert(std::is_base_of_v<EntityComponentBase, ComponentType>);

        return new ComponentType();
    }
    
    virtual void SetupComponent(EntityBase& owner_, const char* name_) = 0;
};