#pragma once

#include "imgui.h"

#define IMGUI_SCOPED_INDENT() \
    ImGui::ScopedIndentType _ScopedIndentType##__LINE__ {};

#define IMGUI_SCOPED_ID(id_) \
    ImGui::ScopedIDType ScopedIDType##__LINE__ { id_ };

namespace ImGui
{
    struct ScopedIndentType
    {
        ScopedIndentType() { Indent(); }
        ~ScopedIndentType() { Unindent(); }
    };

    struct ScopedIDType
    {
        ScopedIDType(const void* id_) { PushID(id_); }
        ~ScopedIDType() { PopID(); }
    };
}