#pragma once

#include <cassert>

inline bool ensure(bool condition_)
{
    assert(condition_);
    return condition_;
}

inline bool ensureMsg(bool condition_, const char* msg_)
{
    assert(condition_ && msg_);
    return condition_;
}