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
    sf::RectangleShape shape { sf::Vector2f{20.f, 20.f} };
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(sf::Vector2f{ 10.f, 10.f });
    shape.setPosition(GetPosition());
    renderTarget_.draw(shape);
}
