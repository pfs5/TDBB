#include "engine/engine.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "imgui.h"
#include "inspectable.h"
#include "level/level.h"
#include "rendering/colors.h"
#include "platform/imgui-SFML.h"

#include <chrono>

Engine::Engine() = default;

void Engine::Init(const EngineSetupParams& params_)
{
    _setupParams = params_;

    for (EngineModuleBase* module : _engineModules)
    {
        module->SetupModule(this);
    }
    
    for (EngineModuleBase* module : _engineModules)
    {
        module->Init();
    }

    if (params_.StartLevel != nullptr)
    {
        params_.StartLevel->SetupLevel();
    }
}

void Engine::Shutdown()
{
    for (EngineModuleBase* module : _engineModules)
    {
        module->Shutdown();
    }
}

void Engine::StartEngine()
{
    // ptodo - current level
    if (_setupParams.StartLevel != nullptr)
    {
        _setupParams.StartLevel->StartLevel();
    }
}

void Engine::StopEngine()
{
    // ptodo - current level
    if (_setupParams.StartLevel != nullptr)
    {
        _setupParams.StartLevel->StopLevel();
    }
}

void Engine::Update(float deltaSeconds_)
{
    UpdateSimulationStats_Update();
    
    UpdateSimulationRequest();
    UpdateSimulation(deltaSeconds_);

    UpdateModules(deltaSeconds_);
}

void Engine::Draw()
{
    UpdateSimulationStats_Draw();

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
        if (ImGui::CollapsingHeader("Stats:", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("FPS: %.2f", _simulationStats.DrawsPerSecond);
            ImGui::Text("UPS: %.2f", _simulationStats.UpdatesPerSecond);
        }
        
        // Draw module debug
        if (ImGui::CollapsingHeader("Modules:", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (EngineModuleBase* module : _engineModules)
            {
                ensure(module != nullptr);

                ImGui::Indent();

                const std::string moduleName = module->GetModuleName();
                if(ImGui::CollapsingHeader(moduleName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    module->DrawEditor();
                }

                ImGui::Unindent();
            }   
        }
    }
    ImGui::End();

    // Window - Inspector
    if (ImGui::Begin("Inspector"))
    {
        if (_currentInspectable != nullptr)
        {
            ImGui::Text("Name: %s", _currentInspectable->GetInspectableName());
            ImGui::Separator();
            _currentInspectable->DrawInspectable();
        }
    }
    ImGui::End();

    
    // Game win
    if (ImGui::Begin("Viewport"))
    {
        // Render target for game draw
        static ImVec2 viewportSize { static_cast<float>(_setupParams.GameWindowSize.x), static_cast<float>(_setupParams.GameWindowSize.y) }; // ptodo - game size
        sf::ContextSettings windowSettings;
        windowSettings.antialiasingLevel = 1;
        _gameRenderTexture.create(static_cast<unsigned int>(viewportSize.x), static_cast<unsigned int>(viewportSize.y));
        _gameRenderTexture.setSmooth(true);
        _gameRenderTexture.clear(sf::Color::Black);

        DrawGame();

        viewportSize = ImGui::GetWindowSize();
        ImGui::Image(_gameRenderTexture);
    }
    ImGui::End();
}

void Engine::UpdateModules(float deltaSeconds_)
{
    for (EngineModuleBase* module : _engineModules)
    {
        ensure(module != nullptr);
        module->Update(deltaSeconds_);
    }
}

void Engine::DrawModules(sf::RenderTarget& renderTarget_)
{
    for (EngineModuleBase* module : _engineModules)
    {
        ensure(module != nullptr);
        module->Draw(renderTarget_);
    }
}

void Engine::DrawGame()
{
    DrawModules(_gameRenderTexture);
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

void Engine::UpdateSimulationStats_Update()
{
    static const float UPDATE_PERIOD_SECONDS = 1.f;
    static auto lastCalculationTimestamp = std::chrono::high_resolution_clock::now();

    ++_simulationStats.UpdateCounter;
    
    const auto currentTimestamp = std::chrono::high_resolution_clock::now();
    
    const auto durationSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimestamp - lastCalculationTimestamp);
    if (static_cast<float>(durationSeconds.count()) * 1e-3f > UPDATE_PERIOD_SECONDS)
    {
        _simulationStats.UpdatesPerSecond = static_cast<float>(_simulationStats.UpdateCounter) / UPDATE_PERIOD_SECONDS;
        _simulationStats.UpdateCounter = 0;
        lastCalculationTimestamp = currentTimestamp;
    }
}

void Engine::UpdateSimulationStats_Draw()
{
    static const float UPDATE_PERIOD_SECONDS = 1.f;
    static auto lastCalculationTimestamp = std::chrono::high_resolution_clock::now();

    ++_simulationStats.DrawCounter;
    
    const auto currentTimestamp = std::chrono::high_resolution_clock::now();
    
    const auto durationSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimestamp - lastCalculationTimestamp);
    if (static_cast<float>(durationSeconds.count()) * 1e-3f > UPDATE_PERIOD_SECONDS)
    {
        _simulationStats.DrawsPerSecond = static_cast<float>(_simulationStats.DrawCounter) / UPDATE_PERIOD_SECONDS;
        _simulationStats.DrawCounter = 0;
        lastCalculationTimestamp = currentTimestamp;
    }
}
