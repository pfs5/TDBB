#include "property.h"

#include "imgui.h"
#include "propertycontainer.h"

void PropertyBase::DrawEditor()
{
    // Default editor should work for most cases. Just shows values, doesn't allow change.
    ImGui::Text("%s", ValueToString().c_str());
}

void PropertyBase::OnValueChanged()
{
    if (_parentContainer != nullptr)
    {
        _parentContainer->OnPropertyChanged(this);
    }
}
