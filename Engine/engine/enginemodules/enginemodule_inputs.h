#pragma once

#include "engine/engineaccess.h"
#include "engine/enginemodule.h"
#include "math/vector.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include <bitset>

class EngineModule_Inputs : public EngineModule<EngineModule_Inputs>, public IEngineInputsAccess
{
    using Super = EngineModule<EngineModule_Inputs>;
    using MouseButtonFlags = std::bitset<(size_t)sf::Mouse::Button::ButtonCount>;

public:
    using Super::Super;

    virtual void Init() override;
    virtual void Shutdown() override;
    
    virtual void Update(float deltaSeconds_) override;
    virtual void DrawEditor() override;
    
    virtual sf::Vector2i GetMousePosition() const override;
    virtual bool IsMouseButtonDown(sf::Mouse::Button mouseButton_) const override;
    virtual bool MouseButtonPressed(sf::Mouse::Button mouseButton_) const override;
    virtual bool MouseButtonReleased(sf::Mouse::Button mouseButton_) const override;
private:
    sf::Vector2i _lastMousePosition = ZERO_VECTOR_I;
    MouseButtonFlags _mouseButtonState;
    MouseButtonFlags _mouseButtonPressed;
    MouseButtonFlags _mouseButtonReleased;
};
