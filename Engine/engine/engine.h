#pragma once

#include "moduleengineaccess.h"
#include "enginemodules/enginemodule_entity.h"
#include "enginemodules/enginemodule_level.h"

#include "enginemodules/enginemodule_inputs.h"
#include "editormodules/editormodule_leveleditor.h"
#include "enginemodules/enginemodule_logger.h"
#include "math/vector.h"

#include <SFML/Graphics/RenderTexture.hpp>

#include "editorengineaccess.h"
#include "enginemodules/enginemodule_debug.h"
#include "enginemodules/enginemodule_editor.h"

class EditorModuleBase;
class Level;

#define ENGINE_MODULE(type_, name_) type_ name_ { _engineModules }
#define EDITOR_MODULE(type_, name_) type_ name_ { _editorModules }

namespace sf
{
    class RenderTarget;
    class RenderWindow;
}

class EngineModuleBase;
class IInspectable;

struct EngineSetupParams
{
    sf::Vector2u GameWindowSize { 0u, 0u};
    sf::RenderWindow* GameWindow = nullptr;
    
    // ptodo
    Level* StartLevel = nullptr;
};

class Engine : public IModuleEngineAccess, public IEditorEngineAccess
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
    virtual uint64_t GetEditorTimeMilliseconds() final;
    virtual sf::Vector2i GetGameWindowPosition() const final { return _gameWindowPosition; }
    virtual const sf::RenderWindow* GetGameWindow() const final { return _setupParams.GameWindow; }

    // IEditorEngineAccess
    virtual const IInspectable* GetCurrentInspected() const final { return _currentInspectable; } 
    virtual void SetCurrentInspected(IInspectable* value_) final { _currentInspectable = value_; _currentInspectableChanged.Broadcast(value_); }
    virtual CurrentInspectableChangedDelegate& CurrentInspectableChanged() final { return _currentInspectableChanged; }
    virtual sf::Vector2u GetGameWindowSize() const final { return _setupParams.GameWindowSize; }
    
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
    ENGINE_MODULE(EngineModule_Inputs, _engineModuleInputs);
    ENGINE_MODULE(EngineModule_Logger, _engineModuleLogger);
    ENGINE_MODULE(EngineModule_Editor, _engineModuleEditor);
    ENGINE_MODULE(EngineModule_Debug, _engineModuleDebug);
    // END Engine modules

    // BEGIN Editor modules
    // note: _editorModules must be defined before all modules, used for very basic reflection.
    std::vector<EditorModuleBase*> _editorModules;
    EDITOR_MODULE(EditorModule_LevelEditor, _engineModuleLevelEditor);
    // END Editor modules

    EngineSetupParams _setupParams;
    
    sf::RenderTexture _gameRenderTexture;

    bool _startSimulationRequested = false;
    bool _stopSimulationRequested = false;
    SimulationState _simulationState;
    SimulationStats _simulationStats;

    // Group under "io" or something
    sf::Vector2i _gameWindowPosition = ZERO_VECTOR_I;
    
    uint64_t _engineStartTimestampMs = 0;
    
    // ptodo - move this functionality to a module
    CurrentInspectableChangedDelegate _currentInspectableChanged;
    IInspectable* _currentInspectable = nullptr;
    
private:
    void SetupModules();
    void InitModules();
    void StartupModules();
    void NotifyModulesEngineStarted();
    void ShutdownModules();
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