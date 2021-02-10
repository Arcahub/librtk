#include "ReplaySubject.hpp"
#include <criterion/assert.h>
#include <criterion/criterion.h>

Test(test_replay_subject_primitive, create)
{
    rtk::ReplaySubject<int> s;
}

Test(test_replay_subject_primitive, next_without_sub)
{
    rtk::ReplaySubject<int> s;

    s.next(1);
}

Test(test_replay_subject_primitive, error_without_sub)
{
    rtk::ReplaySubject<int> s;

    s.error();
}

Test(test_replay_subject_primitive, error_with_already_error_called)
{
    rtk::ReplaySubject<int> s;

    s.error();
    cr_expect_throw(s.error(), std::exception);
}

Test(test_replay_subject_primitive, complete_without_sub)
{
    rtk::ReplaySubject<int> s;

    s.complete();
}

Test(test_replay_subject_primitive, complete_with_already_complete_called)
{
    rtk::ReplaySubject<int> s;

    s.complete();
    cr_expect_throw(s.complete(), std::exception);
}

Test(test_replay_subject_primitive, asObservable)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
}

Test(test_replay_subject_primitive, asObservable_two)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
}

Test(test_replay_subject_primitive, subscribe_empty_one)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();

    obs.subscribe();
}

Test(test_replay_subject_primitive, subscribe_empty_two)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();

    obs.subscribe();
    obs2.subscribe();
}

Test(test_replay_subject_primitive, next_one_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    cr_assert_eq(sum, 0);
    s.next(5);
    cr_assert_eq(sum, 5);
}

Test(test_replay_subject_primitive, next_two_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
    int sum = 0;

    obs2.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    cr_assert_eq(sum, 0);
    s.next(5);
    cr_assert_eq(sum, 10);
}

Test(test_replay_subject_primitive, error_one_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    cr_assert_eq(sum, 0);
    s.error();
    cr_assert_eq(sum, 5);
}

Test(test_replay_subject_primitive, error_two_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
    int sum = 0;

    obs2.subscribe(nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    obs.subscribe(nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    cr_assert_eq(sum, 0);
    s.error();
    cr_assert_eq(sum, 10);
}

Test(test_replay_subject_primitive, complete_one_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    cr_assert_eq(sum, 0);
    s.complete();
    cr_assert_eq(sum, 5);
}

Test(test_replay_subject_primitive, complete_two_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
    int sum = 0;

    obs2.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    cr_assert_eq(sum, 0);
    s.complete();
    cr_assert_eq(sum, 10);
}

Test(test_replay_subject_primitive, unsub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
    int sum = 0;

    obs2.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    s.unsubscribe();
}

Test(test_replay_subject_primitive, unsub_expect_throw_on_next)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
    int sum = 0;

    obs2.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    s.unsubscribe();
    cr_expect_throw(s.next(0), std::exception);
}

Test(test_replay_subject_primitive, unsub_expect_throw_on_error)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
    int sum = 0;

    obs2.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    s.unsubscribe();
    cr_expect_throw(s.error(), std::exception);
}

Test(test_replay_subject_primitive, unsub_expect_throw_on_complete)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
    int sum = 0;

    obs2.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    s.unsubscribe();
    cr_expect_throw(s.complete(), std::exception);
}

Test(test_replay_subject_primitive, replay_one)
{
    rtk::ReplaySubject<int> s;
    int sum = 0;

    s.next(5);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    cr_assert_eq(sum, 5);
}

Test(test_replay_subject_primitive, replay_multi)
{
    rtk::ReplaySubject<int> s;
    int sum = 0;

    s.next(5);
    s.next(10);
    s.next(15);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    cr_assert_eq(sum, 30);
}

Test(test_replay_subject_primitive, replay_setbuffsize)
{
    rtk::ReplaySubject<int> s(12);
    int sum = 0;

    s.next(5);
    s.next(10);
    s.next(15);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    cr_assert_eq(sum, 30);
}

Test(test_replay_subject_primitive, replay_overflow_buffsize)
{
    rtk::ReplaySubject<int> s(2);
    int sum = 0;

    s.next(5);
    s.next(10);
    s.next(15);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    cr_assert_eq(sum, 25);
}