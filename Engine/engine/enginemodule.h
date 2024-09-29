#pragma once
#include "editorengineaccess.h"
#include "util/typeid.h"

namespace sf
{
    class RenderTarget;
}

class IEditorEngineAccess;
class IModuleEngineAccess;

class EngineModuleBase
{
public:
    // Used for basic reflection on all registered modules
    EngineModuleBase(std::vector<EngineModuleBase*>& container_)
    {
        container_.push_back(this);
    }

    virtual ~EngineModuleBase() = default;

    // Currently we give modules editor access as well.
    void SetupModule(IModuleEngineAccess* engineAccess_, IEditorEngineAccess* editorAccess_)
    {
        _engineAccess = engineAccess_;
        _editorAccess = editorAccess_;
    }
    
    virtual void Init() {}
    virtual void Startup() {}   // Called as the engine is started. All Inits were called at this point.
    virtual void OnEngineStarted() {} // Called after the engine started. Everything in the engine should be ready and started at this point.
    
    virtual void Shutdown() {}

    virtual void Update(float deltaSeconds_) {}
    virtual void Draw(sf::RenderTarget& renderTarget_) {}
    
    virtual std::string GetModuleName() const = 0;
    virtual void DrawEditor() {}

protected:
    // Engine module is contained inside the engine so, after setup, this is never null or invalid.
    IModuleEngineAccess* _engineAccess = nullptr;
    IEditorEngineAccess* _editorAccess = nullptr;
};

template <typename CRTP>
class EngineModule : public EngineModuleBase
{
    using Super = EngineModuleBase;
    
public:
    using Super::Super;
    virtual std::string GetModuleName() const final { return TypeId<CRTP>::GetName(); }
};
