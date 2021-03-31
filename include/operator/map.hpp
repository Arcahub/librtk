#if !defined(RKT_MAP_HPP)
#define RKT_MAP_HPP

#include "Observable.hpp"

namespace rtk {

template <typename T, typename U>
Operator<T, U> map(std::function<U(T)> fmap)
{
    return [=](Observable<T> obs) {
        return Observable<U>([=](std::shared_ptr<Subscriber<U>> subscriber) mutable {
            obs.subscribe(
                [=](T value) { subscriber->next(fmap(value)); },
                [=]() { subscriber->error(); },
                [=]() { subscriber->complete(); });
        });
    };
}

}
#endif // RKT_MAP_HPP
