#pragma once

#include "engine/entity/entity.h"
#include "engine/entity/entitycomponent.h"
#include "engine/entity/entitycomponents/entitycomponent_physics.h"

class EntityComponent_Test : public EntityComponent<EntityComponent_Test>
{
    using Super = EntityComponent<EntityComponent_Test>;

public:
    // ptodo - testing inputs
    virtual void SetupInputs() override;

private:
    void Input_Turn();
    void Input_Up(float axisValue_);
    void Input_Right(float axisValue_);
};

class Entity_Basic : public Entity<Entity_Basic>
{
    using Super = Entity<Entity_Basic>;
    
public:
    Entity_Basic()
    {
        ConstructComponent<EntityComponent_Physics>("Physics");

        // ptodo - testing
        SetBoundsSize(80.f, 80.f);
    }

    virtual void Update(float deltaSeconds_) override;
    virtual void Draw(sf::RenderTarget& renderTarget_) override;
};

class Entity_Basic2 : public Entity<Entity_Basic2>
{
public:
    Entity_Basic2()
    {
        ConstructComponent<EntityComponent_Physics>("Physics");
    }
};
