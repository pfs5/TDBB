#pragma once

class IPropertyContainer
{
public:
    virtual void OnPropertyChanged(PropertyBase* property_) {}
};