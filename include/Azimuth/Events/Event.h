#pragma once
#include <Azimuth/Common.h>

namespace Azimuth
{
    template <typename... Args>
    class Event
    {
    public:
        template <typename T>
        void Attach(T *instance, void (T::*callback)(Args...))
        {
            m_CallbackFunctions.emplace_back([instance, callback](Args... args)
                                             { (instance->*callback)(args...); });
        }

        void Attach(void (*callback)(Args...))
        {
            m_CallbackFunctions.emplace_back(callback);
        }
        void Detach(void (*callback)(Args...))
        {
            m_CallbackFunctions.remove(callback);
        }
        void Dispatch(Args... args)
        {
            for (auto &callback : m_CallbackFunctions)
            {
                callback(args...);
            }
        }

    protected:
        std::list<std::function<void(Args...)>> m_CallbackFunctions;
    };
}