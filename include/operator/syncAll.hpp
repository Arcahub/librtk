#if !defined(SYNC_ALL_HPP)
#define SYNC_ALL_HPP

#include "Observable.hpp"
#include "Subscriber.hpp"
#include <iostream>
#include <vector>

namespace rtk {
template <typename T>
Observable<std::shared_ptr<std::vector<T>>> syncAll(std::vector<Observable<T>> inputs)
{
    return Observable<std::shared_ptr<std::vector<T>>>([inputs](std::shared_ptr<Subscriber<std::shared_ptr<std::vector<T>>>> subscriber) {
        std::shared_ptr<std::vector<size_t>> counter(new std::vector<size_t>(inputs.size()));
        std::shared_ptr<std::vector<T>> results(new std::vector<T>(inputs.size()));
        size_t index = 0;

        for (Observable<T> obs : inputs) {
            obs.subscribe([results, index, counter, subscriber](T value) {
                size_t current = 0;

                (*counter)[index] += 1;
                (*results)[index] = value;
                current = (*counter)[index];
                if (std::find_if(counter->begin(), counter->end(), [current](size_t value) { return value < current; }) == counter->end()) {
                    subscriber->next(results);
                } },
                [index, counter, subscriber]() {
                    counter->erase(counter->begin() + index);
                    subscriber->error();
                },
                [index, counter, subscriber]() {
                    counter->erase(counter->begin() + index);
                    if (counter->empty())
                        subscriber->complete();
                });
            index++;
        }
    });
}
} // namespace rtk

#endif // SYNC_ALL_HPP
