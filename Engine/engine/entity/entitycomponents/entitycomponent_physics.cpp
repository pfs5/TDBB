#include "engine/entity/entitycomponents/entitycomponent_physics.h"

#include "engine/entity/entity.h"
#include "serialization/serialization.h"

#include "imgui.h"

IMPLEMENT_ENTITY_COMPONENT(EntityComponent_Physics);

void EntityComponent_Physics::Update(float deltaSeconds_)
{
    EntityComponent<EntityComponent_Physics>::Update(deltaSeconds_);

    GetOwner().SetPosition(GetOwner().GetPosition() + *_velocity * deltaSeconds_);

    // ptodo - testing
    sf::Vector2f pos = GetOwner().GetPosition();
    if (pos.x > 600.f || pos.x < 50.f)
    {
        _velocity->x *= -1.f;   // ptodo - figure out how to allow this kind of access
    }
    if (pos.y > 600.f || pos.y < 50.f)
    {
        _velocity->y *= -1.f;   // ptodo - figure out how to allow this kind of access
    }

    pos.x = std::clamp(pos.x, 50.f, 600.f);
    pos.y = std::clamp(pos.y, 50.f, 600.f);
    GetOwner().SetPosition(pos);
}
