#pragma once

#include "moduleengineaccess.h"
#include "enginemodules/enginemodule_entity.h"
#include "enginemodules/enginemodule_level.h"

#include <SFML/Graphics/RenderTexture.hpp>

class Level;

#define ENGINE_MODULE(type_, name_) type_ name_ { _engineModules }

namespace sf
{
    class RenderTarget;
}

class EngineModuleBase;

struct EngineSetupParams
{
    sf::Vector2u GameWindowSize { 0u, 0u};

    // ptodo
    Level* StartLevel = nullptr;
};

class Engine : public IModuleEngineAccess
{
public:
    Engine();
    
    void Init(const EngineSetupParams& params_);
    void Shutdown();

    void StartEngine();
    void StopEngine();
    
    void Update(float deltaSeconds_);
    void Draw();

    // IModuleEngineAccess
    virtual bool IsSimulationActive() const final { return _simulationState.SimulationActive; }
    virtual const IInspectable* GetCurrentInspected() const final { return _currentInspectable; } 
    virtual void SetCurrentInspected(IInspectable* value_) final { _currentInspectable = value_; }
    
// ptodo - move simulation logic into module
    
private:
    struct SimulationState
    {
        bool SimulationActive = false;
        float SimulationTimeSeconds = 0.f;
    };

    struct SimulationStats
    {
        float DrawsPerSecond = 0.f;
        float UpdatesPerSecond = 0.f;

        uint32_t DrawCounter = 0;
        uint32_t UpdateCounter = 0;
    };
    
private:
    // BEGIN Engine modules.
    // note: _engineModules must be defined before all modules, used for very basic reflection.
    std::vector<EngineModuleBase*> _engineModules;
    ENGINE_MODULE(EngineModule_Level, _engineModuleLevel);
    ENGINE_MODULE(EngineModule_Entity, _engineModuleEntity);
    // END Engine modules

    EngineSetupParams _setupParams;
    
    sf::RenderTexture _gameRenderTexture;

    bool _startSimulationRequested = false;
    bool _stopSimulationRequested = false;
    SimulationState _simulationState;
    SimulationStats _simulationStats;

    // ptodo - move this functionality to a module
    IInspectable* _currentInspectable = nullptr;
    
private:
    void UpdateModules(float deltaSeconds_);
    void DrawModules(sf::RenderTarget& renderTarget_);
    
    void DrawGame();

    void RequestStartSimulation() { _startSimulationRequested = true; }
    void RequestStopSimulation() { _stopSimulationRequested = true; }
    void UpdateSimulationRequest();

    void StartSimulation();
    void StopSimulation();
    void UpdateSimulation(float deltaSeconds_);

    void UpdateSimulationStats_Update();
    void UpdateSimulationStats_Draw();

    void Save();
};