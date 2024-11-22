#include "engine/entity/property.h"
#include "imgui.h"
#include "engine/object/baseobject.h"

// ptodo - onchange

#define IMPLEMENT_EDITOR_INTEGRAL_TYPE(type_) \
template<> \
void Property<type_>::DrawEditor() \
{ \
    int value = static_cast<int>(_value); \
    if (ImGui::InputInt(StringFormat("##%s", GetName()).c_str(), &value)) \
    { \
        _value = static_cast<type_>(value); \
    } \
} \

// bool
template<>
void Property<bool>::DrawEditor()
{
    if (ImGui::Checkbox(StringFormat("##%s", GetName()).c_str(), &_value))
    {
        // ptodo - onchange        
    }
}

// Integral types
IMPLEMENT_EDITOR_INTEGRAL_TYPE(uint8_t);
IMPLEMENT_EDITOR_INTEGRAL_TYPE(uint16_t);
IMPLEMENT_EDITOR_INTEGRAL_TYPE(uint32_t);
IMPLEMENT_EDITOR_INTEGRAL_TYPE(uint64_t);

IMPLEMENT_EDITOR_INTEGRAL_TYPE(int8_t);
IMPLEMENT_EDITOR_INTEGRAL_TYPE(int16_t);
IMPLEMENT_EDITOR_INTEGRAL_TYPE(int32_t);
IMPLEMENT_EDITOR_INTEGRAL_TYPE(int64_t);

// float
template<>
void Property<float>::DrawEditor()
{
    if (ImGui::InputFloat(StringFormat("##%s", GetName()).c_str(), &_value, 0, 0, "%.2f"))
    {
    }
}

// sf::Vector2f
template<>
void Property<sf::Vector2f>::DrawEditor()
{
    float inValues [] = { _value.x, _value.y };
    if(ImGui::InputFloat2(StringFormat("##%s", GetName()).c_str(), inValues, "%.2f"))
    {
        Set(sf::Vector2f { inValues[0], inValues[1] });
    }
}

// std::string
template<>
void Property<std::string>::DrawEditor()
{
    char buffer [256];
    std::memcpy(buffer, _value.c_str(), _value.size() + 1);
    if (ImGui::InputText(StringFormat("##%s", GetName()).c_str(), buffer, IM_ARRAYSIZE(buffer)))
    {
        _value = buffer;
    }
}

// const Class
template<>
void Property<const Class*>::DrawEditor()
{
    if (_value != nullptr)
    {
        ImGui::Text("%s (#%d)", _value->GetName().c_str(), _value->GetNameHash());        
    }
    else
    {
        ImGui::Text("-");
    }
}
    

#undef IMPLEMENT_EDITOR_INTEGRAL_TYPE