#if !defined(RTK_UTILS)
#define RTK_UTILS

#include "Observable.hpp"

namespace rtk {
template <typename T>
Observable<T> of(std::vector<T> vec)
{
    return Observable<T>([vec](std::shared_ptr<Subscriber<T>> subscriber) {
        for (T v : vec) {
            subscriber->next(v);
        }
        subscriber->complete();
    });
}
}

#endif // RTK_UTILS
