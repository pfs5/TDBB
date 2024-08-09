#pragma once
#include "util/typeid.h"

class EngineModuleBase
{
public:
    // Used for basic reflection on all registered modules
    EngineModuleBase(std::vector<EngineModuleBase*>& container_) { container_.push_back(this); }

    virtual void Init() {}
    virtual void Shutdown() {}
    
    virtual std::string GetModuleName() const = 0;
    virtual void DrawDebug() const {}
};

template <typename CRTP>
class EngineModule : public EngineModuleBase
{
    using Super = EngineModuleBase;
    
public:
    using Super::Super;
    virtual std::string GetModuleName() const override { return TypeId<CRTP>::GetName(); }
};
