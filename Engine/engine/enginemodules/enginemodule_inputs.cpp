#include "enginemodule_inputs.h"

#include <SFML/Window/Mouse.hpp>

#include "engine/moduleengineaccess.h"
#include "errorhandling/assert.h"
#include "imgui/imguihelpers.h"

#include <SFML/Graphics/RenderWindow.hpp>

void EngineModule_Inputs::Init()
{
    Super::Init();

    EngineInputsAccess::Open(*this);
}

void EngineModule_Inputs::Shutdown()
{
    EngineInputsAccess::Close(*this);
    
    Super::Shutdown();
}

void EngineModule_Inputs::Update(float deltaSeconds_)
{
    Super::Update(deltaSeconds_);

    ensure(_engineAccess->GetGameWindow() != nullptr);
    const sf::Vector2i gameWindowPosition = _engineAccess->GetGameWindowPosition();
    _lastMousePosition = sf::Mouse::getPosition(*_engineAccess->GetGameWindow()) - gameWindowPosition;

    const MouseButtonFlags lastMouseButtonState { _mouseButtonState };
    for (size_t i = 0; i < sf::Mouse::ButtonCount; ++i)
    {
        const sf::Mouse::Button button = static_cast<sf::Mouse::Button>(i);
        _mouseButtonState.set(i, sf::Mouse::isButtonPressed(button));

        _mouseButtonPressed.set(i, _mouseButtonState.test(i) && !lastMouseButtonState.test(i));
        _mouseButtonReleased.set(i, !_mouseButtonState.test(i) && lastMouseButtonState.test(i));
    }
}

void EngineModule_Inputs::DrawEditor()
{
    Super::DrawEditor();

    IMGUI_SCOPED_INDENT();
    
    ImGui::Text("Mouse position: %d %d", _lastMousePosition.x, _lastMousePosition.y);

    ImGui::NewLine();
    
    // Mouse buttons
    ImGui::SeparatorText("Left button");
    constexpr size_t leftButtonIdx = static_cast<size_t>(sf::Mouse::Button::Left);
    ImGui::Text("state: %d | pressed: %d | released: %d",
        _mouseButtonState.test(leftButtonIdx), _mouseButtonPressed.test(leftButtonIdx), _mouseButtonReleased.test(leftButtonIdx));

    ImGui::SeparatorText("Right button");
    constexpr size_t rightButtonIdx = static_cast<size_t>(sf::Mouse::Button::Right);
    ImGui::Text("state: %d | pressed: %d | released: %d",
        _mouseButtonState.test(rightButtonIdx), _mouseButtonPressed.test(rightButtonIdx), _mouseButtonReleased.test(rightButtonIdx));
}

sf::Vector2i EngineModule_Inputs::GetMousePosition() const
{
    return _lastMousePosition;
}

bool EngineModule_Inputs::IsMouseButtonDown(sf::Mouse::Button mouseButton_) const
{
    return _mouseButtonState[static_cast<size_t>(mouseButton_)];
}

bool EngineModule_Inputs::MouseButtonPressed(sf::Mouse::Button mouseButton_) const
{
    return _mouseButtonPressed[static_cast<size_t>(mouseButton_)];
}

bool EngineModule_Inputs::MouseButtonReleased(sf::Mouse::Button mouseButton_) const
{
    return _mouseButtonReleased[static_cast<size_t>(mouseButton_)];
}
