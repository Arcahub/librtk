#if !defined(RTK_REPLAY_SUBJECT_HPP)
#define RTK_REPLAY_SUBJECT_HPP

#include "Subject.hpp"
#include <algorithm>
#include <deque>
#include <queue>
#include <stdint.h>

namespace rtk {
template <typename T>
class ReplaySubject : public Subject<T> {

public:
    ReplaySubject(size_t bufferSize = 0)
    {
        if (bufferSize == 0)
            bufferSize = m_buffer.max_size();
        m_bufferSize = bufferSize;
    };

    ~ReplaySubject() = default;

    void next(T value)
    {
        {
            std::lock_guard<std::mutex> l(_mtx);

            if (!this->m_isStopped) {
                if (m_buffer.size() == m_bufferSize)
                    m_buffer.pop_back();
                m_buffer.push_front(value);
            }
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
            if (!sub->isClosed()) {
                std::for_each(m_buffer.begin(), m_buffer.end(), [&subscriber](T value) {
                    subscriber->next(value);
                });
            }
        }
        return sub;
    }

private:
    size_t m_bufferSize;
    std::deque<T> m_buffer;
    mutable std::mutex _mtx;
};
}

#endif // RTK_REPLAY_SUBJECT_HPP
