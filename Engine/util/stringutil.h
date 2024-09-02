#pragma once

#include <memory>
#include <string>
#include <string_view>

struct RotatingStringBuffer
{
    static constexpr size_t BUFFER_SIZE = 1024;

    // ptodo - this needs better naming
    char Buffer [BUFFER_SIZE];
    size_t CurrentStart = 0;
    
    char* GetSlot(size_t size_)
    {
        ensure(size_ <= BUFFER_SIZE);
        if (BUFFER_SIZE - CurrentStart < size_)
        {
            // Reset to start
            CurrentStart = 0;
        }

        char* slot = Buffer + CurrentStart;
        CurrentStart += size_;
        return slot;
    }
};

template<typename ... Args>
std::string StringFormat(const char* format, Args... args)
{
    static RotatingStringBuffer buffer;

    const int size_s = std::snprintf(nullptr, 0, format, args ...) + 1; // Extra space for '\0'
    const size_t size = static_cast<size_t>(size_s);

    char* bufferSlot = buffer.GetSlot(size);
    std::snprintf(bufferSlot, size, format, args ...);
    return { bufferSlot, bufferSlot + size - 1 }; // We don't want the '\0' inside
}

inline std::string ToString(bool value)
{
    return value ? "true" : "false";
}

template<typename T>
std::string ToString(const T& value)
{
    return "Please define a ToString specialization";
}