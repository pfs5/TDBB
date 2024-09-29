#pragma once

#include <chrono>
#include <cstdint>

namespace Time
{
    inline uint64_t NowMs()
    {
        using namespace std::chrono;
        milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        return ms.count();
    }
}
