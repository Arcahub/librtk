#ifndef RTK_OBSERVABLE_HPP_
#define RTK_OBSERVABLE_HPP_

#include <memory>
#include <mutex>

#include "Operator.hpp"
#include "Subscribable.hpp"
#include "Subscriber.hpp"

namespace rtk {
template <typename T>
class Observable : public Subscribable<T> {
public:
    Observable(
        std::function<void(std::shared_ptr<Subscriber<T>>)> subscribe = nullptr)
    {
        m_subscribe = subscribe;
        m_source = nullptr;
    };

    Observable(const Observable<T>& obs)
    {
        m_source = obs.m_source;
        m_subscribe = obs.m_subscribe;
    }

    Observable<T>& operator=(const Observable<T>& obs)
    {
        m_source = obs.m_source;
        m_subscribe = obs.m_subscribe;
        return *this;
    }

    virtual ~Observable() = default;

    std::shared_ptr<Subscription> subscribe(
        std::function<void(T)> onNext = nullptr,
        std::function<void()> onError = nullptr,
        std::function<void()> onComplete = nullptr)
    {
        std::shared_ptr<Subscriber<T>>
            subscriber(new Subscriber<T>(onNext, onError, onComplete));

        subscriber->add(_subscribe(subscriber));
        return subscriber;
    }

    template <typename U>
    Observable<U> operator>>(Operator<T, U> op)
    {
        return op(*this);
    }

protected:
    Observable<T> _createChild()
    {
        Observable<T> obs;

        obs.m_source = this;
        return obs;
    };

    virtual std::shared_ptr<Subscription> _subscribe(std::shared_ptr<Subscriber<T>> subscriber)
    {
        std::lock_guard<std::mutex> l(_mtx);

        if (m_subscribe != nullptr) {
            m_subscribe(subscriber);
            return Subscription::empty();
        }
        if (m_source)
            return m_source->subscribe([&, this, subscriber](T value) { subscriber->next(value); }, [&, this, subscriber]() { subscriber->error(); },
                [&, this, subscriber]() {
                    subscriber->complete();
                });
        return Subscription::empty();
    };
    Observable<T>* m_source;
    std::function<void(std::shared_ptr<Subscriber<T>>)> m_subscribe;

private:
    mutable std::mutex _mtx;
};
}

#endif /* !OBSERVABLE_HPP_ */
