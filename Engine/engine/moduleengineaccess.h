#pragma once

class IInspectable;

// Interface used to expose engine functionality to engine modules.
class IModuleEngineAccess
{
    // ptodo - make private and make module the friend
public:
    virtual bool IsSimulationActive() const = 0;
    
    virtual const IInspectable* GetCurrentInspected() const = 0; 
    virtual void SetCurrentInspected(IInspectable* value_) = 0; 
};