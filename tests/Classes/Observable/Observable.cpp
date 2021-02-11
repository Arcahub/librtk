#include "Observable.hpp"
#include "Subscriber.hpp"
#include <criterion/criterion.h>

Test(test_observable, empty_observable)
{
    rtk::Observable<int> obs;

    obs.subscribe();
}

Test(test_observable, with_subscriber_next)
{
    rtk::Observable<int> obs([](std::shared_ptr<rtk::Subscriber<int>> subscriber) {
        subscriber->next(5);
    });
    int sum = 0;

    obs.subscribe([&sum](int value) {
        sum += value;
    });
    cr_assert_eq(sum, 5);
}

Test(test_observable, with_subscriber_error)
{
    rtk::Observable<int> obs([](std::shared_ptr<rtk::Subscriber<int>> subscriber) {
        subscriber->error();
    });
    int sum = 0;

    obs.subscribe(nullptr, [&sum]() {
        sum += 5;
    });
    cr_assert_eq(sum, 5);
}

Test(test_observable, with_subscriber_complete)
{
    rtk::Observable<int> obs([](std::shared_ptr<rtk::Subscriber<int>> subscriber) {
        subscriber->complete();
    });
    int sum = 0;

    obs.subscribe(nullptr, nullptr, [&sum]() {
        sum += 5;
    });
    cr_assert_eq(sum, 5);
}

Test(test_observable, with_subscriber_next_and_complete)
{
    rtk::Observable<int> obs([](std::shared_ptr<rtk::Subscriber<int>> subscriber) {
        subscriber->next(5);
        subscriber->complete();
    });
    int sum = 0;

    obs.subscribe([&sum](int value) { sum += value; }, nullptr, [&sum]() { sum += 5; });
    cr_assert_eq(sum, 10);
}

rtk::Observable<int> test_op(rtk::Observable<int> obs)
{
    return obs;
}

Test(test_observable, pipe)
{
    rtk::Observable<int> obs;
    rtk::Operator<int, int> op = test_op;

    obs = obs >> op;
}