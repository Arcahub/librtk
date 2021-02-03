#ifndef RTK_SUBSCRIBER_HPP_
#define RTK_SUBSCRIBER_HPP_

#include "Observer.hpp"
#include "Subscription.hpp"
#include <mutex>

namespace rtk {
template <typename T>
class Subscriber : public Observer<T>, public Subscription {
public:
    Subscriber(
        std::function<void(T&)> onNext = nullptr,
        std::function<void()> onError = nullptr,
        std::function<void()> onComplete = nullptr)
        : Subscription()
    {
        m_next = onNext;
        m_error = onError;
        m_complete = onComplete;
    }
    ~Subscriber() {};

    void next(T& value)
    {
        if (m_next)
            m_next(value);
    }
    void error()
    {
        if (m_error)
            m_error();
    }
    void complete()
    {
        if (m_complete)
            m_complete();
    }

protected:
    bool _isStopped = false;
    std::function<void()> m_error;
    std::function<void()> m_complete;
    std::function<void(T&)> m_next;
};
}

#endif // RTK_SUBSCRIBER
