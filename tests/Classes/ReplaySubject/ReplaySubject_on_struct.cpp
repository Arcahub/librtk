#include "rtk/ReplaySubject.hpp"
#include <criterion/assert.h>
#include <criterion/criterion.h>

typedef struct Test_s {
    int val;
} Test_t;

Test(test_replay_subject_struct, create)
{
    rtk::ReplaySubject<Test_t> s;
}

Test(test_replay_subject_struct, next_without_sub)
{
    rtk::ReplaySubject<Test_t> s;
    Test_t v = { 1 };

    s.next(v);
}

Test(test_replay_subject_struct, error_without_sub)
{
    rtk::ReplaySubject<Test_t> s;

    s.error();
}

Test(test_replay_subject_struct, error_with_already_error_called)
{
    rtk::ReplaySubject<Test_t> s;

    s.error();
    cr_expect_throw(s.error(), std::exception);
}

Test(test_replay_subject_struct, complete_without_sub)
{
    rtk::ReplaySubject<Test_t> s;

    s.complete();
}

Test(test_replay_subject_struct, complete_with_already_complete_called)
{
    rtk::ReplaySubject<Test_t> s;

    s.complete();
    cr_expect_throw(s.complete(), std::exception);
}

Test(test_replay_subject_struct, asObservable)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
}

Test(test_replay_subject_struct, asObservable_two)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
}

Test(test_replay_subject_struct, subscribe_empty_one)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();

    obs.subscribe();
}

Test(test_replay_subject_struct, subscribe_empty_two)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();

    obs.subscribe();
    obs2.subscribe();
}

Test(test_replay_subject_struct, next_one_sub)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    cr_assert_eq(sum.val, 0);
    s.next({ 5 });
    cr_assert_eq(sum.val, 5);
}

Test(test_replay_subject_struct, next_two_sub)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    cr_assert_eq(sum.val, 0);
    s.next({ 5 });
    cr_assert_eq(sum.val, 10);
}

Test(test_replay_subject_struct, error_one_sub)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    cr_assert_eq(sum.val, 0);
    s.error();
    cr_assert_eq(sum.val, 5);
}

Test(test_replay_subject_struct, error_two_sub)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    obs.subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    cr_assert_eq(sum.val, 0);
    s.error();
    cr_assert_eq(sum.val, 10);
}

Test(test_replay_subject_struct, sub_on_already_error)
{
    rtk::ReplaySubject<Test_t> s;
    Test_t sum = { 0 };

    s.error();
    s.asObservable().subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    cr_assert_eq(sum.val, 5);
}

Test(test_replay_subject_struct, sub_on_already_complete)
{
    rtk::ReplaySubject<Test_t> s;
    Test_t sum = { 0 };

    s.complete();
    s.asObservable().subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    cr_assert_eq(sum.val, 5);
}

Test(test_replay_subject_struct, complete_one_sub)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    cr_assert_eq(sum.val, 0);
    s.complete();
    cr_assert_eq(sum.val, 5);
}

Test(test_replay_subject_struct, complete_two_sub)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    cr_assert_eq(sum.val, 0);
    s.complete();
    cr_assert_eq(sum.val, 10);
}

Test(test_replay_subject_struct, unsub)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    s.unsubscribe();
}

Test(test_replay_subject_struct, unsub_expect_throw_on_next)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    s.unsubscribe();
    cr_expect_throw(s.next({ 0 }), std::exception);
}

Test(test_replay_subject_struct, unsub_expect_throw_on_error)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    s.unsubscribe();
    cr_expect_throw(s.error(), std::exception);
}

Test(test_replay_subject_struct, unsub_expect_throw_on_complete)
{
    rtk::ReplaySubject<Test_t> s;
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    s.unsubscribe();
    cr_expect_throw(s.complete(), std::exception);
}

Test(test_replay_subject_on_struct, replay_one)
{
    rtk::ReplaySubject<Test_t> s;
    int sum = 0;

    s.next({ 5 });
    s.asObservable().subscribe([&sum](Test_t value) {
        sum += value.val;
    });
    cr_assert_eq(sum, 5);
}

Test(test_replay_subject_on_struct, replay_multi)
{
    rtk::ReplaySubject<Test_t> s;
    int sum = 0;

    s.next({ 5 });
    s.next({ 10 });
    s.next({ 15 });
    s.asObservable().subscribe([&sum](Test_t value) {
        sum += value.val;
    });
    cr_assert_eq(sum, 30);
}

Test(test_replay_subject_on_struct, replay_setbuffsize)
{
    rtk::ReplaySubject<Test_t> s(12);
    int sum = 0;

    s.next({ 5 });
    s.next({ 10 });
    s.next({ 15 });
    s.asObservable().subscribe([&sum](Test_t value) {
        sum += value.val;
    });
    cr_assert_eq(sum, 30);
}

Test(test_replay_subject_on_struct, replay_overflow_buffsize)
{
    rtk::ReplaySubject<Test_t> s(2);
    int sum = 0;

    s.next({ 5 });
    s.next({ 10 });
    s.next({ 15 });
    s.asObservable().subscribe([&sum](Test_t value) {
        sum += value.val;
    });
    cr_assert_eq(sum, 25);
}