#pragma once

#include <functional>

// Base class contains API accessible to objects not owning the delegate.
template<typename... Args>
class NonOwnedDelegate
{
    template<typename ObjectType>
    using FunctionPointer = void (ObjectType::*)(Args...);
    
public:
    template<typename ObjectType>
    void AddListener(ObjectType* object_, FunctionPointer<ObjectType> callback_)
    {
        _callbacks.emplace_back(object_, [object_, callback_](Args&&... args_)
        {
            std::invoke(callback_, object_, std::forward<Args>(args_)...);
        });
    }
    void RemoveListeners(void* object_)
    {
        _callbacks.erase(std::remove_if(_callbacks.begin(), _callbacks.end(),
            [object_](const Callback& cb_) { return cb_.Object == object_; })
        );
    }

    void Clear()
    {
        _callbacks.clear();
    }

protected:
    struct Callback
    {
        void* Object;
        std::function<void(Args...)> Function;
    };

protected:
    std::vector<Callback> _callbacks;

};

// ptodo - currently just uses std::function for everything
template<typename... Args>
class Delegate : public NonOwnedDelegate<Args...>
{
    using NonOwnedDelegate<Args...>::Callback;
    using NonOwnedDelegate<Args...>::_callbacks;

public:
    using NonOwnedType = NonOwnedDelegate<Args...>;
    
public:
    void Broadcast(const Args&... args_) const
    {
        for (const Callback& callback : _callbacks)
        {
            callback.Function(std::forward<const Args>(args_)...);
        }
    }

};
