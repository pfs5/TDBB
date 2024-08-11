#pragma once

#include "engine/engineaccess.h"
#include "engine/enginemodule.h"

class EngineModule_Level : public EngineModule<EngineModule_Level>, public IEngineLevelAccess
{
    using Super = EngineModule<EngineModule_Level>;

public:
    using Super::Super;
    
    virtual void Init() override;
    virtual void Shutdown() override;
};