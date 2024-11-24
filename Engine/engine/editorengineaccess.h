#pragma once

#include "events/delegate.h"

#include <SFML/System/Vector2.hpp>

class IInspectable;

// Used to provide an interface of the engine to the editor modules.
class IEditorEngineAccess
{
public:
    using CurrentInspectableChangedDelegate = Delegate<IInspectable* /* newInspectable_ */>;

    virtual const IInspectable* GetCurrentInspected() const = 0; 
    virtual void SetCurrentInspected(IInspectable* value_) = 0;
    virtual CurrentInspectableChangedDelegate& CurrentInspectableChanged() = 0;

    virtual sf::Vector2u GetGameWindowSize() const = 0;
};