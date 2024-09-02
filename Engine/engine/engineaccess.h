#pragma once

#include "entity/entitymemory.h"
#include "errorhandling/assert.h"
#include "util/proxy.h"

#include "SFML/System/Vector2.hpp"

// Use these interfaces and proxies to access engine features.

class EntityBase;

// Level access
class IEngineLevelAccess
{
    virtual void LoadLevel(const char* path_) = 0;
};

struct EngineLevelAccess : public Proxy<IEngineLevelAccess>
{
    EngineLevelAccess();
};

// Entity access
class IEngineEntityAccess
{
public:
    template <typename EntityType>
    EntityType* SpawnEntity(const char* name_, sf::Vector2f position_)
    {
        // static_assert(std::is_base_of_v<EntityBase, EntityType>);
        static_assert(true);
        
        EntityType* entity = EntityMemory::InstantiateEntityObject<EntityType>();
        ensure(entity != nullptr);

        entity->SetEntityName(name_);
        
        SetupEntity(*entity, position_);
        entity->EntityCreated();
        
        return entity;
    }

protected:
    virtual void SetupEntity(EntityBase& entity_, sf::Vector2f position_) = 0;
};

struct EngineEntityAccess : public Proxy<IEngineEntityAccess>
{
    
};