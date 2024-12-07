#include "engine/engine.h"

#include "imgui.h"
#include "inspectable.h"
#include "rendering/colors.h"
#include "platform/imgui-SFML.h"

#include "imgui/imguihelpers.h"
#include "object/baseobject.h"
#include "util/stringutil.h"
#include "util/time.h"

#include <SFML/Graphics/RenderTexture.hpp>

#include <chrono>

Engine::Engine() = default;

void Engine::Init(const EngineSetupParams& params_)
{
    _setupParams = params_;

    SetupModules();
    InitModules();
    
    if (params_.StartLevel != nullptr)
    {
        // params_.StartLevel->SetupLevel();
    }
}

void Engine::Shutdown()
{
    ShutdownModules();
}

void Engine::StartEngine()
{
    StartupModules();
    
    _engineStartTimestampMs = Time::NowMs();

    NotifyModulesEngineStarted();
    
    // ptodo - current level
    if (_setupParams.StartLevel != nullptr)
    {
        // _setupParams.StartLevel->StartLevel();
    }
}

void Engine::StopEngine()
{
    // ptodo - current level
    if (_setupParams.StartLevel != nullptr)
    {
        // _setupParams.StartLevel->StopLevel();
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
        ImGui::BeginTable("##ToolbarTable", 3, ImGuiTableFlags_Borders);

        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100.f);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 300.f);
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

        // 3 - Load/Save
        ImGui::TableSetColumnIndex(2);
        const bool saveButtonDisabled = !_engineModuleLevel.IsCurrentLevelDirty();
        ImGui::BeginDisabled(saveButtonDisabled);
        if (ImGui::Button("Save", ImVec2{80.f, 0.f}))
        {
            Save();
        }
        ImGui::EndDisabled();

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
                IMGUI_SCOPED_INDENT();

                ensure(module != nullptr);

                const std::string moduleName = module->GetModuleName();
                if(ImGui::CollapsingHeader(moduleName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    module->DrawEditor();
                }
            }   
        }
        
        // Draw editors debug
        if (ImGui::CollapsingHeader("Editors:", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (EditorModuleBase* module : _editorModules)
            {
                IMGUI_SCOPED_INDENT();

                ensure(module != nullptr);

                const std::string moduleName = module->GetModuleName();
                if(ImGui::CollapsingHeader(moduleName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    module->DrawEditor();
                }
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

    // ptodo
    // Window - Classes
    if (ImGui::Begin("Classes"))
    {
        if (ImGui::CollapsingHeader("Registered classes:", ImGuiTreeNodeFlags_DefaultOpen))
        {
            IMGUI_SCOPED_INDENT();

            for (EObjectCategory category : EObjectCategoryIterable{})
            {
                const char* const categoryName = ToString(category);
                const std::string title = StringFormat("%s:", categoryName);
                if (ImGui::CollapsingHeader(title.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    IMGUI_SCOPED_INDENT();
                    
                    const std::vector<Class>& classes = ObjectRepository::GetClasses(category);
                    for (size_t classIdx = 0; classIdx < classes.size(); ++classIdx)
                    {
                        const Class& c = classes[classIdx];
                        ImGui::Text("[%02lld] %s", classIdx, c.GetName().c_str());
                    }
                }
            }
        }
    }
    ImGui::End();

    // Window - Logger
    if (ImGui::Begin("Logger"))
    {
        _engineModuleLogger.DrawWindow();
    }
    ImGui::End();
    
    // Game win
    if (ImGui::Begin("Viewport"))
    {
        const ImVec2 windowPos = ImGui::GetWindowPos();
        
        // Render target for game draw
        static ImVec2 viewportSize { static_cast<float>(_setupParams.GameWindowSize.x), static_cast<float>(_setupParams.GameWindowSize.y) }; // ptodo - game size
        sf::ContextSettings windowSettings;
        windowSettings.antialiasingLevel = EngineSettingsAccess::Get().GetRenderSettings().AntialiasingLevel;
        
        // ptodo - should we call create every time? can something be cached?
        _gameRenderTexture.create(static_cast<unsigned int>(viewportSize.x), static_cast<unsigned int>(viewportSize.y), windowSettings);
        _gameRenderTexture.setSmooth(true);
        _gameRenderTexture.clear(sf::Color::Black);

        DrawGame();

        _gameRenderTexture.display();

        viewportSize = ImGui::GetWindowSize();

        // Viewport menu bar
        ImGui::BeginTable("##ViewportMenuBar", 2, ImGuiTableFlags_Borders);

        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 100.f);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(1);
        ImGui::BeginDisabled(_engineModuleLevelEditor.GetCurrentManipulationMode() == EditorModule_LevelEditor::EManipulationMode::Position);
        if(ImGui::Button("pos."))
        {
            _engineModuleLevelEditor.SetCurrentManipulationMode(EditorModule_LevelEditor::EManipulationMode::Position);
        }
        ImGui::EndDisabled();
        
        ImGui::SameLine();
        ImGui::BeginDisabled(_engineModuleLevelEditor.GetCurrentManipulationMode() == EditorModule_LevelEditor::EManipulationMode::Rotation);
        if(ImGui::Button("rot."))
        {
            _engineModuleLevelEditor.SetCurrentManipulationMode(EditorModule_LevelEditor::EManipulationMode::Rotation);
        }
        ImGui::EndDisabled();
        
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("manipulator: ");
        ImGui::SameLine();

        ImGui::EndTable();
        
        const ImVec2 gameRenderTexturePos = ImGui::GetCursorPos();
        ImGui::Image(_gameRenderTexture);

        // We have to use the absolute pos of the window and also the local pos of the texture where we render the game.
        _gameWindowPosition.x = static_cast<int>(windowPos.x + gameRenderTexturePos.x);
        _gameWindowPosition.y = static_cast<int>(windowPos.y + gameRenderTexturePos.y);
    }
    ImGui::End();
}

uint64_t Engine::GetEditorTimeMilliseconds()
{
    return Time::NowMs() - _engineStartTimestampMs;
}

void Engine::SetupModules()
{
    for (EngineModuleBase* module : _engineModules)
    {
        module->SetupModule(this, this);
    }
    for (EditorModuleBase* module : _editorModules)
    {
        module->SetupModule(this);
    }
}

void Engine::InitModules()
{
    
    for (EngineModuleBase* module : _engineModules)
    {
        module->Init();
    }
    for (EditorModuleBase* module : _editorModules)
    {
        module->Init();
    }
}

void Engine::StartupModules()
{
    for (EngineModuleBase* const module : _engineModules)
    {
        module->Startup();
    }

    for (EditorModuleBase* const module : _editorModules)
    {
        module->Startup();
    }
}

void Engine::NotifyModulesEngineStarted()
{
    for (EngineModuleBase* const module : _engineModules)
    {
        module->OnEngineStarted();
    }
}

void Engine::ShutdownModules()
{
    for (EngineModuleBase* module : _engineModules)
    {
        module->Shutdown();
    }
    for (EditorModuleBase* module : _editorModules)
    {
        module->Shutdown();
    }
}

void Engine::UpdateModules(float deltaSeconds_)
{
    for (EngineModuleBase* module : _engineModules)
    {
        ensure(module != nullptr);
        module->Update(deltaSeconds_);
    }

    for (EditorModuleBase* module : _editorModules)
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
    
    for (EditorModuleBase* module : _editorModules)
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

void Engine::Save()
{
    _engineModuleLevel.SaveCurrentLevel();
}
