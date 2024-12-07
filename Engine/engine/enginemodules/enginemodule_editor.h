#pragma once

#include "engine/engineaccess.h"
#include "engine/enginemodule.h"

class EngineModule_Editor : public EngineModule<EngineModule_Editor>, public IEngineSettingsAccess
{
    using Super = EngineModule<EngineModule_Editor>;

public:
    using Super::Super;

    // EngineModule
    virtual void Init() final;
    virtual void Shutdown() final;
    virtual void DrawEditor() final;
    
    // IEngineSettingsAccess
    virtual const RenderSettings& GetRenderSettings() const final { return _renderSettings; }
    
private:
    RenderSettings _renderSettings;
};
