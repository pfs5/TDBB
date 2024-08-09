#include "pch.h"
#include "engine/engine.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "enginesetup.h"
#include "imgui.h"
#include "rendering/colors.h"

void Engine::Init()
{
    for (EngineModuleBase* module : _engineModules)
    {
        module->Init();
    }
}

void Engine::Shutdown()
{
    for (EngineModuleBase* module : _engineModules)
    {
        module->Shutdown();
    }
}

void Engine::Update(float deltaSeconds_)
{
    UpdateSimulationRequest();
    UpdateSimulation(deltaSeconds_);
}

void Engine::Draw()
{
    // Render target for game draw
    static ImVec2 viewportSize { static_cast<float>(WINDOW_SIZE.x), static_cast<float>(WINDOW_SIZE.y) }; // ptodo - game size

    // Toolbar
    if (ImGui::Begin("Toolbar"))
    {
        ImGui::BeginTable("##ToolbarTable", 2, ImGuiTableFlags_Borders);

        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100.f);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();

        // 1 - Buttons
        ImGui::TableSetColumnIndex(0);
        if (_simulationState.SimulationActive)
        {
            if (ImGui::Button("Stop", ImVec2{80.f, 0.f}))
            {
                RequestStopSimulation();
            }
        }
        else
        {
            if(ImGui::Button("Simulate", ImVec2{80.f, 0.f}))
            {
                RequestStartSimulation();
            }
        }
        
        // 2 - Simulation state
        ImGui::TableSetColumnIndex(1);
        if (_simulationState.SimulationActive)
        {
            ImGui::TextColored(ToImVec4(Colors::LightGreen), "Simulation active. Time = %.2fs", _simulationState.SimulationTimeSeconds);
        }
        else
        {
            ImGui::Text("Simulation stopped");
        }

        ImGui::EndTable();
    }
    ImGui::End();
    
    // Window - ImGuiDemo
    ImGui::ShowDemoWindow();

    // Window - Engine
    if (ImGui::Begin("Engine"))
    {
        // ptodo - to fun

        // Draw module debug
        if (ImGui::CollapsingHeader("Modules:", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (const EngineModuleBase* module : _engineModules)
            {
                ensure(module != nullptr);

                ImGui::Indent();

                const std::string moduleName = module->GetModuleName();
                if(ImGui::CollapsingHeader(moduleName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    module->DrawDebug();
                }

                ImGui::Unindent();
            }   
        }
    }
    ImGui::End();

    // Game win

    if (ImGui::Begin("Viewport"))
    {
        _gameRenderTexture.create(viewportSize.x, viewportSize.y);
        _gameRenderTexture.clear(sf::Color::Black);

        DrawGame();

        viewportSize = ImGui::GetWindowSize();
        ImGui::Image(_gameRenderTexture);
    }
    ImGui::End();
}

void Engine::DrawGame()
{
    sf::CircleShape shape {100.f};
    shape.setFillColor(sf::Color::Yellow);
    _gameRenderTexture.draw(shape);
}

void Engine::UpdateSimulationRequest()
{
    if (_simulationState.SimulationActive)
    {
        if (_stopSimulationRequested)
        {
            StopSimulation();
        }
    }
    else if (_startSimulationRequested)
    {
        StartSimulation();
    }

    _startSimulationRequested = false;
    _stopSimulationRequested = false;
}

void Engine::StartSimulation()
{
    ensure(!_simulationState.SimulationActive);

    _simulationState.SimulationTimeSeconds = 0.f;
    _simulationState.SimulationActive = true;
}

void Engine::StopSimulation()
{
    ensure(_simulationState.SimulationActive);

    _simulationState.SimulationActive = false;
    _simulationState.SimulationTimeSeconds = 0.f;
}

void Engine::UpdateSimulation(float deltaSeconds_)
{
    _simulationState.SimulationTimeSeconds += deltaSeconds_;
}
