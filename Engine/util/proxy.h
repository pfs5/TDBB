#pragma once

template<typename T>
class Proxy
{
public:
    static T& Get() { return *_instance; }
    static bool IsValid() { return _instance != nullptr; }
    static void Open(T& value)
    {
        assert((void("Opening a proxy without previously closing."), _instance == nullptr));
        _instance = &value;
    }
    static void Close(T& value)
    {
        assert((void("Proxy was opened with a different object."), _instance == &value));
        _instance = nullptr;
    }

private:
    static T* _instance;

};

template<typename T>
T* Proxy<T>::_instance = nullptr;
