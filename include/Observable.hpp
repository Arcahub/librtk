#ifndef RTK_OBSERVABLE_HPP_
#define RTK_OBSERVABLE_HPP_

#include <memory>
#include <mutex>

#include "Subscribable.hpp"
#include "Subscriber.hpp"

namespace rtk {
template <typename T>
class Observable : public Subscribable<T> {
public:
    Observable(
        std::function<void(std::shared_ptr<Subscription>)> subscribe = nullptr)
    {
        m_subscribe = subscribe;
        m_source = nullptr;
    };

    ~Observable() {};

    std::shared_ptr<Subscription> subscribe(
        std::function<void(T&)> onNext = nullptr,
        std::function<void()> onError = nullptr,
        std::function<void()> onComplete = nullptr)
    {
        std::shared_ptr<Subscriber<T>> subscriber(new Subscriber<T>(onNext, onError, onComplete));

        subscriber->add(_subscribe(subscriber));
        return subscriber;
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
        if (m_subscribe != nullptr) {
            m_subscribe(subscriber);
            return Subscription::empty();
        }
        if (m_source)
            return m_source->subscribe([&, this, subscriber](T& value) { subscriber->next(value); }, [&, this, subscriber]() { subscriber->error(); },
                [&, this, subscriber]() {
                    subscriber->complete();
                });
        return Subscription::empty();
    };
    Observable<T>* m_source;
    std::function<void(std::shared_ptr<Subscription>)> m_subscribe;
};
}

#endif /* !OBSERVABLE_HPP_ */
