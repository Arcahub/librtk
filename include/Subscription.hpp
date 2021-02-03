#ifndef RTK_SUBSCRIPTION_HPP_
#define RTK_SUBSCRIPTION_HPP_

#include <algorithm>
#include <functional>
#include <vector>

namespace rtk {
class Subscription {
public:
    Subscription(std::function<void()> teardown = nullptr)
    {
        if (teardown)
            m_teardowns.push_back(teardown);
    }
    ~Subscription() { }

    void unsubscribe()
    {
        if (m_closed) {
            return;
        }
        m_closed = true;
        for (auto teardown : m_teardowns)
            teardown();
        for (auto teardown : m_subscriptions)
            teardown->unsubscribe();
    };
    void add(std::function<void()> teardown)
    {
        if (m_closed) {
            teardown();
            return;
        }
        m_teardowns.push_back(teardown);
    }

    void add(std::shared_ptr<Subscription> teardown)
    {
        if (teardown.get() == this)
            return;
        if (m_closed) {
            if (!teardown->m_closed)
                teardown->unsubscribe();
            return;
        }
        if (teardown->m_closed
            || !(std::find(m_subscriptions.begin(), m_subscriptions.end(), teardown)
                == m_subscriptions.end())) {
            return;
        }
        teardown->addParent(std::shared_ptr<Subscription>(this));
        m_subscriptions.push_back(teardown);
    }

    static std::shared_ptr<Subscription> empty()
    {
        std::shared_ptr<Subscription> sub(new Subscription());

        sub->m_closed = true;
        return sub;
    }

protected:
private:
    void addParent(std::shared_ptr<Subscription> parent)
    {
        m_parent.push_back(parent);
    }

    bool m_closed = false;
    std::vector<std::function<void()>> m_teardowns;
    std::vector<std::shared_ptr<Subscription>> m_subscriptions;
    std::vector<std::shared_ptr<Subscription>> m_parent;
};
}

#endif /* !SUBSCRIPTION_HPP_ */
