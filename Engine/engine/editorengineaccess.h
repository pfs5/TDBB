#pragma once

#include "events/delegate.h"

class IInspectable;

// Used to provide an interface of the engine to the editor modules.
class IEditorEngineAccess
{
public:
    using CurrentInspectableChangedDelegate = Delegate<IInspectable* /* newInspectable_ */>;

    virtual const IInspectable* GetCurrentInspected() const = 0; 
    virtual void SetCurrentInspected(IInspectable* value_) = 0;
    virtual CurrentInspectableChangedDelegate& CurrentInspectableChanged() = 0;
};