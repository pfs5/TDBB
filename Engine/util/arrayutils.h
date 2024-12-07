#pragma once

#include <vector>

template <typename T>
void RemoveAtSwap(std::vector<T>& vector_, size_t index_)
{
    vector_[index_] = std::move(vector_[vector_.size() - 1]);
    vector_.erase(vector_.end() - 1);
}