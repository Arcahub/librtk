#ifndef RTK_BEHAVIOR_SUBJECT_HPP_
#define RTK_BEHAVIOR_SUBJECT_HPP_

#include "Subject.hpp"

namespace rtk {
template <typename T>
class BehaviorSubject : public Subject<T> {
public:
    BehaviorSubject(T value) { m_value = value; };
    ~BehaviorSubject() = default;

    T getValue() const
    {
        return m_value;
    }

    void next(T value)
    {
        {
            std::lock_guard<std::mutex> l(_mtx);

            m_value = value;
        }
        Subject<T>::next(value);
    }

protected:
    std::shared_ptr<Subscription> _subscribe(std::shared_ptr<Subscriber<T>> subscriber) override
    {
        std::shared_ptr<Subscription> sub;
        {
            std::lock_guard<std::mutex> l(_mtx);

            sub = Subject<T>::_subscribe(subscriber);
            if (!sub->isClosed())
                subscriber->next(m_value);
        }
        return sub;
    }

private:
    T m_value;
    mutable std::mutex _mtx;
};
}

#endif /* !RTK_BEHAVIOR_SUBJECT_HPP_ */
