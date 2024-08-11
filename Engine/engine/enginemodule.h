#pragma once
#include "util/typeid.h"

namespace sf
{
    class RenderTarget;
}

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

    void SetupModule(IModuleEngineAccess* engineAccess_) { _engineAccess = engineAccess_; }
    
    virtual void Init() {}
    virtual void Shutdown() {}

    virtual void Update(float deltaSeconds_) {}
    virtual void Draw(sf::RenderTarget& renderTarget_) {}
    
    virtual std::string GetModuleName() const = 0;
    virtual void DrawEditor() {}

protected:
    // Engine module is contained inside the engine so, after setup, this is never null or invalid.
    IModuleEngineAccess* _engineAccess = nullptr;
};

template <typename CRTP>
class EngineModule : public EngineModuleBase
{
    using Super = EngineModuleBase;
    
public:
    using Super::Super;
    virtual std::string GetModuleName() const final { return TypeId<CRTP>::GetName(); }
};
