#include "pch.h"

#include <iostream>

#include "imgui.h"
#include "platform/imgui-SFML.h"
#include "enginesetup.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "engine/engine.h"

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE.x, WINDOW_SIZE.y}), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    
    ImGui::SFML::Init(window);

    Engine engine;
    engine.Init();
    
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        const sf::Time deltaTime = deltaClock.restart();
        const float deltaSeconds = deltaTime.asSeconds();
        
        ImGui::SFML::Update(window, deltaTime);
        engine.Update(deltaSeconds);

        ImGui::DockSpaceOverViewport();
        
        engine.Draw();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    engine.Shutdown();
    ImGui::SFML::Shutdown();

    return 0;
}
