#if !defined(SYNC_ALL_HPP)
#define SYNC_ALL_HPP

#include "rtk/Observable.hpp"
#include "rtk/Subscriber.hpp"
#include <queue>
#include <vector>

namespace rtk {
template <typename T>
Observable<std::vector<T>> syncAll(std::vector<Observable<T>> inputs)
{
    return Observable<std::vector<T>>([inputs](std::shared_ptr<Subscriber<std::vector<T>>> subscriber) {
        std::shared_ptr<std::vector<std::queue<T>>> results(new std::vector<std::queue<T>>(inputs.size()));
        size_t index = 0;

        for (Observable<T> obs : inputs) {
            obs.subscribe([=](T value) {
                (*results)[index].push(value);
                if (std::find_if(results->begin(), results->end(), [](auto value) { return value.empty(); }) == results->end()) {
                    std::vector<T> return_vec(results->size());
                    size_t index = 0;

                    for (auto& q : *results) {
                        return_vec[index] = q.front();
                        q.pop();
                        index++;
                    }
                    subscriber->next(return_vec);
                } },
                [=]() {
                    results->erase(results->begin() + index);
                    subscriber->error();
                },
                [=]() {
                    results->erase(results->begin() + index);
                    if (results->empty())
                        subscriber->complete();
                });
            index++;
        }
    });
}
} // namespace rtk

#endif // SYNC_ALL_HPP
