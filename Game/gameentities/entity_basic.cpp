#include "gameentities/entity_basic.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

IMPLEMENT_ENTITY(Entity_Basic);
IMPLEMENT_ENTITY(Entity_Basic2);
IMPLEMENT_ENTITY_COMPONENT(EntityComponent_Test);

void Entity_Basic::Update(float deltaSeconds_)
{
    Super::Update(deltaSeconds_);
}

void Entity_Basic::Draw(sf::RenderTarget& renderTarget_)
{
    Super::Draw(renderTarget_);

    // ptodo - testing
    sf::RectangleShape shapeBase { sf::Vector2f{ 50.f, 50.f } };
    shapeBase.setFillColor(sf::Color::Yellow);
    shapeBase.setOrigin(sf::Vector2f{ 25.f, 25.f });
    shapeBase.setRotation(GetRotationDeg());
    shapeBase.setPosition(GetPosition());
    renderTarget_.draw(shapeBase);

    sf::RectangleShape shapeBottom { sf::Vector2f{ 50.f, 10.f } };
    shapeBottom.setFillColor(sf::Color::Red);
    shapeBottom.setOrigin(sf::Vector2f{ 25.f, -25.f });
    shapeBottom.setRotation(GetRotationDeg());
    shapeBottom.setPosition(GetPosition());
    renderTarget_.draw(shapeBottom);
}
