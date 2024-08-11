#include "levels/level_showcase.h"

#include "engine/engineaccess.h"
#include "gameentities/entity_basic.h"

#include "SFML/System/Vector2.hpp"

void Level_Showcase::SetupLevel()
{
    Level::SetupLevel();

    //
    EngineEntityAccess::Get().SpawnEntity<Entity_Basic>("Test entity 1", sf::Vector2f{ 200.f, 300.f });
    EngineEntityAccess::Get().SpawnEntity<Entity_Basic>("Test entity 2", sf::Vector2f{ 400.f, 200.f });
}

void Level_Showcase::StartLevel()
{
    Level::StartLevel();
    
}

void Level_Showcase::StopLevel()
{
    Level::StopLevel();
    
}
