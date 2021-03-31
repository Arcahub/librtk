#ifndef RTK_SUBSCRIBABLE_HPP_
#define RTK_SUBSCRIBABLE_HPP_

#include "Subscription.hpp"
#include <functional>

namespace rtk {
template <typename T>
class Subscribable {
public:
    virtual ~Subscribable() {};
    virtual std::shared_ptr<Subscription> subscribe(
        std::function<void(T)> onNext = nullptr,
        std::function<void()> onError = nullptr,
        std::function<void()> onComplete = nullptr)
        = 0;
};
}

#endif /* !SUBSCRIBABLE_HPP_ */
