#pragma once

#include "engine/entity/entitycomponent.h"

#include <SFML/System/Vector2.hpp>

class EntityComponent_Physics : public EntityComponent<EntityComponent_Physics>
{
public:
    virtual void Update(float deltaSeconds_) override;
    virtual void DrawInspectable() override;

    // ISerializable
    virtual void Serialize(nlohmann::json& data_) const override;
    virtual bool Deserialize(const char* fileName_, const nlohmann::json& data_) override;
    
private:
    sf::Vector2f _velocity = sf::Vector2f{ 0.f, 0.f };
};