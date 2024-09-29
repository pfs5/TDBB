#pragma once

#include "util/typeid.h"

#include <string>
#include <vector>

class IEditorEngineAccess;

namespace sf
{
    class RenderTarget;
}

class EditorModuleBase
{
public:
    // Container used for basic reflection inside of the engine.
    EditorModuleBase(std::vector<EditorModuleBase*>& container_)
    {
        container_.emplace_back(this);
    }

    virtual ~EditorModuleBase() = default;

    void SetupModule(IEditorEngineAccess* engineAccess_) { _engineAccess = engineAccess_; }

    // Callbacks
    virtual void Init() {}
    virtual void Startup() {}
    virtual void Shutdown() {}

    virtual void Update(float deltaSeconds_) {}
    virtual void Draw(sf::RenderTarget& renderTarget_) {}

    virtual std::string GetModuleName() const = 0;
    virtual void DrawEditor() {}
    
protected:
    IEditorEngineAccess* _engineAccess;
};

// Base class for soft separation of editor logic from the engine.
template <typename CRTP>
class EditorModule : public EditorModuleBase
{
    using Super = EditorModuleBase;

public:
    using Super::Super;
    
    virtual std::string GetModuleName() const final { return TypeId<CRTP>::GetName(); }

};