#ifndef RTK_SUBJECT_HPP_
#define RTK_SUBJECT_HPP_

#include "Observable.hpp"
#include "Subscription.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include <mutex>

namespace rtk {
template <typename T>
class Subject : public Observable<T> {

public:
    Subject()
        : Observable<T>() {};
    virtual ~Subject() = default;

    virtual void next(T value)
    {
        std::lock_guard<std::mutex> l(_mtx);

        throwIfClosed();
        if (!this->m_isStopped) {
            std::for_each(m_observers.begin(), m_observers.end(), [&, value](std::shared_ptr<Observer<T>> obs) mutable {
                obs->next(value);
            });
        }
    }

    void error()
    {
        std::lock_guard<std::mutex> l(_mtx);

        throwIfClosed();
        if (!this->m_isStopped) {
            m_isStopped = m_hasError = true;
            for (std::shared_ptr<Observer<T>> v : m_observers) {
                v->error();
            }
            m_observers.clear();
        }
    }

    void complete()
    {
        std::lock_guard<std::mutex> l(_mtx);

        throwIfClosed();
        if (!this->m_isStopped) {
            m_isStopped = true;
            for (std::shared_ptr<Observer<T>> v : m_observers) {
                v->complete();
            }
            m_observers.clear();
        }
    }

    void unsubscribe()
    {
        std::lock_guard<std::mutex> l(_mtx);

        m_isStopped = m_closed = true;
        m_observers.clear();
    }

    Observable<T> asObservable()
    {
        return this->_createChild();
    }

protected:
    virtual std::shared_ptr<Subscription> _subscribe(std::shared_ptr<Subscriber<T>> subscriber) override
    {
        {
            std::lock_guard<std::mutex> l(_mtx);

            throwIfClosed();
            if (m_hasError) {
                subscriber->error();
            } else if (m_isStopped)
                subscriber->complete();
        }
        return _innerSubscribe(subscriber);
    }

    std::shared_ptr<Subscription> _innerSubscribe(std::shared_ptr<Subscriber<T>> subscriber)
    {
        {
            std::lock_guard<std::mutex> l(_mtx);

            if (m_hasError || m_isStopped)
                return Subscription::empty();
            m_observers.push_back(subscriber);
        }
        return std::shared_ptr<Subscription>(new Subscription([&, this, subscriber]() {
            std::lock_guard<std::mutex> l(_mtx);

            m_observers.erase(std::find_if(m_observers.begin(), m_observers.end(), [&subscriber](std::shared_ptr<Observer<T>> obs) {
                return (obs.get() == subscriber.get());
            }));
        }));
    }

    void throwIfClosed() const
    {
        if (m_closed)
            throw std::exception();
    }

    bool m_isStopped = false;

private:
    std::vector<std::shared_ptr<Observer<T>>> m_observers;
    bool m_closed = false;
    bool m_hasError = false;
    mutable std::mutex _mtx;
};
}

#endif /* !SUBJECT_HPP_ */
