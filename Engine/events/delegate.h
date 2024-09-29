#pragma once

#include <functional>

// ptodo - currently just uses std::function for everything
template<typename... Args>
class Delegate
{
    template<typename ObjectType>
    using FunctionPointer = void (ObjectType::*)(Args...);
    
public:
    void Broadcast(const Args&... args_) const;
    
    template<typename ObjectType>
    void AddListener(ObjectType* object_, FunctionPointer<ObjectType> callback_);
    void RemoveListeners(void* object_);

    void Clear();

private:
    struct Callback
    {
        void* Object;
        std::function<void(Args...)> Function;
    };

    std::vector<Callback> _callbacks;
};

template <typename ... Args>
void Delegate<Args...>::Broadcast(const Args&... args_) const
{
    for (const Callback& callback : _callbacks)
    {
        callback.Function(std::forward<const Args>(args_)...);
    }
}

template <typename ... Args>
template <typename ObjectType>
void Delegate<Args...>::AddListener(ObjectType* object_, FunctionPointer<ObjectType> callback_)
{
    _callbacks.emplace_back(object_, [object_, callback_](Args&&... args_)
    {
        std::invoke(callback_, object_, std::forward<Args>(args_)...);
    });
}

template <typename ... Args>
void Delegate<Args...>::RemoveListeners(void* object_)
{
    _callbacks.erase(std::remove_if(_callbacks.begin(), _callbacks.end(),
            [object_](const Callback& cb_) { return cb_.Object == object_; })
        );
}

template <typename ... Args>
void Delegate<Args...>::Clear()
{
    _callbacks.clear();
}
