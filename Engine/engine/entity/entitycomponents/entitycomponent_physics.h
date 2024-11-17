#pragma once

#include "engine/entity/entitycomponent.h"

#include <SFML/System/Vector2.hpp>

#include "math/vector.h"

class EntityComponent_Physics : public EntityComponent<EntityComponent_Physics>
{
public:
    virtual void Update(float deltaSeconds_) override;

private:
    COMPONENT_PROPERTY(EntityComponent_Physics, sf::Vector2f, _velocity, "Velocity", ZERO_VECTOR_F);
    COMPONENT_PROPERTY(EntityComponent_Physics, sf::Vector2f, _newthingie, "NewThingie", ZERO_VECTOR_F);
};
