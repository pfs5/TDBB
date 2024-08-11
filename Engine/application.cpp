#include "application.h"

#include "engine/engine.h"

#include "imgui.h"
#include "platform/imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

void Application::Run(const ApplicationSetupParams& params_)
{
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({params_.GameWindowSizeX, params_.GameWindowSizeY}),
        "TDBB",
        sf::Style::Default,
        windowSettings
        );
    // window.setFramerateLimit(60);
    
    ImGui::SFML::Init(window);

    // Init style - ptodo
    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2{ 2.f, 4.f };
    
    Engine engine;

    EngineSetupParams params;
    params.GameWindowSize = sf::Vector2u{ params_.GameWindowSizeX, params_.GameWindowSizeY };
    params.StartLevel = params_.StartLevel;
    
    engine.Init(params);
    engine.StartEngine();
    
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

    engine.StopEngine();
    engine.Shutdown();
    ImGui::SFML::Shutdown();
}
