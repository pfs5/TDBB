#include "enginemodule_logger.h"

#include "engine/moduleengineaccess.h"
#include "imgui/imguihelpers.h"

void EngineModule_Logger::Init()
{
    Super::Init();

    EngineLogAccess::Open(*this);
}

void EngineModule_Logger::Shutdown()
{
    EngineLogAccess::Close(*this);
    
    Super::Shutdown();
}

void EngineModule_Logger::Log(const char* string_)
{
    _entries.emplace_back(_engineAccess->GetEditorTimeMilliseconds(), string_);
}

void EngineModule_Logger::LogKey(uint32_t key_, const char* string_)
{
    auto it = std::find_if(_entriesPerKey.begin(), _entriesPerKey.end(),
        [key_](const std::pair<uint32_t, LogEntry>& pair)
        {
            return pair.first == key_;
        });

    if (it == _entriesPerKey.end())
    {
        _entriesPerKey.emplace_back(key_, LogEntry{ _engineAccess->GetEditorTimeMilliseconds(), string_ });
    }
    else
    {
        it->second.Timestamp = _engineAccess->GetEditorTimeMilliseconds();
        it->second.String = string_;
    }
}

void EngineModule_Logger::DrawWindow()
{
    if (ImGui::TreeNodeEx("Log", ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (const LogEntry& entry : _entries)
        {
            const uint32_t milliseconds = static_cast<uint32_t>(entry.Timestamp % 1000);
            const uint32_t seconds = static_cast<uint32_t>((entry.Timestamp / 1000) % 60);
            const uint32_t minutes = static_cast<uint32_t>((entry.Timestamp / 1000 / 60));
            ImGui::Text("[%02d:%02d:%03d] %s",
                minutes, seconds, milliseconds, entry.String.c_str());
        }    
    }

    if (ImGui::TreeNodeEx("Log key", ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (const std::pair<uint32_t, LogEntry>& entryPair : _entriesPerKey)
        {
            const LogEntry& entry = entryPair.second;
            
            const uint32_t milliseconds = static_cast<uint32_t>(entry.Timestamp % 1000);
            const uint32_t seconds = static_cast<uint32_t>((entry.Timestamp / 1000) % 60);
            const uint32_t minutes = static_cast<uint32_t>((entry.Timestamp / 1000 / 60));
            ImGui::Text("[%02d:%02d:%03d] %s",
                minutes, seconds, milliseconds, entry.String.c_str());
        }    
    }
}
