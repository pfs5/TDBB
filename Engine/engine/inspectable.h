#pragma once

class IInspectable
{
public:
    virtual const char* GetInspectableName() const = 0;
    virtual void DrawInspectable() {}
};