#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

#include "enginemodules/enginemodule_level.h"

#define ENGINE_MODULE(type_, name_) type_ name_ { _engineModules }

namespace sf
{
    class RenderTarget;
}

class EngineModuleBase;

class Engine
{
public:
    void Init();
    void Shutdown();

    void Update(float deltaSeconds_);
    void Draw();

// ptodo - move simulation logic into module
    
private:
    struct SimulationState
    {
        bool SimulationActive = false;
        float SimulationTimeSeconds = 0.f;
    };
    
private:
    // Engine modules. note: _engineModules must be defined before all modules, used for very basic reflection.
    std::vector<EngineModuleBase*> _engineModules;
    ENGINE_MODULE(EngineModule_Level, _engineModuleLevel);
    
    
    sf::RenderTexture _gameRenderTexture;

    bool _startSimulationRequested = false;
    bool _stopSimulationRequested = false;
    SimulationState _simulationState;
    
private:
    void DrawGame();

    void RequestStartSimulation() { _startSimulationRequested = true; }
    void RequestStopSimulation() { _stopSimulationRequested = true; }
    void UpdateSimulationRequest();

    void StartSimulation();
    void StopSimulation();
    void UpdateSimulation(float deltaSeconds_);
};