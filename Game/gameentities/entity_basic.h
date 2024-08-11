#pragma once

#include "engine/entity/entity.h"
#include "engine/entity/entitycomponent.h"
#include "engine/entity/entitycomponents/entitycomponent_physics.h"

class EntityComponent_Test : public EntityComponent<EntityComponent_Test>
{
    using Super = EntityComponent<EntityComponent_Test>;
};

class Entity_Basic : public Entity<Entity_Basic>
{
public:
    Entity_Basic()
    {
        ConstructComponent<EntityComponent_Physics>("Physics");
    }
};
