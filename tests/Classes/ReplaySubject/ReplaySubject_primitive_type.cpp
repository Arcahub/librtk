#include "rtk/ReplaySubject.hpp"
#include "gtest/gtest.h"

TEST(test_replay_subject_primitive, create)
{
    rtk::ReplaySubject<int> s;
}

TEST(test_replay_subject_primitive, next_without_sub)
{
    rtk::ReplaySubject<int> s;

    s.next(1);
}

TEST(test_replay_subject_primitive, error_without_sub)
{
    rtk::ReplaySubject<int> s(0);

    s.error();
}

TEST(test_replay_subject_primitive, error_with_already_error_called)
{
    rtk::ReplaySubject<int> s(0);
    auto obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.error();
    ASSERT_EQ(sum, 5);
    s.error();
    ASSERT_EQ(sum, 5);
}

TEST(test_replay_subject_primitive, error_on_closed)
{
    rtk::ReplaySubject<int> s(0);

    s.unsubscribe();
    EXPECT_THROW(s.error(), std::exception);
}

TEST(test_replay_subject_primitive, complete_without_sub)
{
    rtk::ReplaySubject<int> s(0);

    s.complete();
}

TEST(test_replay_subject_primitive, complete_with_already_complete_called)
{
    rtk::ReplaySubject<int> s(0);
    auto obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.complete();
    ASSERT_EQ(sum, 5);
    s.complete();
    ASSERT_EQ(sum, 5);
}

TEST(test_replay_subject_primitive, complete_on_closed)
{
    rtk::ReplaySubject<int> s(0);

    s.unsubscribe();
    EXPECT_THROW(s.complete(), std::exception);
}

TEST(test_replay_subject_primitive, asObservable)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
}

TEST(test_replay_subject_primitive, asObservable_two)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
}

TEST(test_replay_subject_primitive, subscribe_empty_one)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();

    obs.subscribe();
}

TEST(test_replay_subject_primitive, subscribe_empty_two)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();

    obs.subscribe();
    obs2.subscribe();
}

TEST(test_replay_subject_primitive, next_one_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    ASSERT_EQ(sum, 0);
    s.next(5);
    ASSERT_EQ(sum, 5);
}

TEST(test_replay_subject_primitive, next_two_sub)
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
    ASSERT_EQ(sum, 0);
    s.next(5);
    ASSERT_EQ(sum, 10);
}

TEST(test_replay_subject_primitive, error_one_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.error();
    ASSERT_EQ(sum, 5);
}

TEST(test_replay_subject_primitive, error_two_sub)
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
    ASSERT_EQ(sum, 0);
    s.error();
    ASSERT_EQ(sum, 10);
}

TEST(test_replay_subject_primitive, complete_one_sub)
{
    rtk::ReplaySubject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.complete();
    ASSERT_EQ(sum, 5);
}

TEST(test_replay_subject_primitive, complete_two_sub)
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
    ASSERT_EQ(sum, 0);
    s.complete();
    ASSERT_EQ(sum, 10);
}

TEST(test_replay_subject_primitive, unsub)
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

TEST(test_replay_subject_primitive, unsub_expect_throw_on_next)
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
    EXPECT_THROW(s.next(0), std::exception);
}

TEST(test_replay_subject_primitive, unsub_expect_throw_on_error)
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
    EXPECT_THROW(s.error(), std::exception);
}

TEST(test_replay_subject_primitive, unsub_expect_throw_on_complete)
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
    EXPECT_THROW(s.complete(), std::exception);
}

TEST(test_replay_subject_primitive, replay_one)
{
    rtk::ReplaySubject<int> s;
    int sum = 0;

    s.next(5);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    ASSERT_EQ(sum, 5);
}

TEST(test_replay_subject_primitive, replay_multi)
{
    rtk::ReplaySubject<int> s;
    int sum = 0;

    s.next(5);
    s.next(10);
    s.next(15);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    ASSERT_EQ(sum, 30);
}

TEST(test_replay_subject_primitive, replay_setbuffsize)
{
    rtk::ReplaySubject<int> s(12);
    int sum = 0;

    s.next(5);
    s.next(10);
    s.next(15);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    ASSERT_EQ(sum, 30);
}

TEST(test_replay_subject_primitive, replay_overflow_buffsize)
{
    rtk::ReplaySubject<int> s(2);
    int sum = 0;

    s.next(5);
    s.next(10);
    s.next(15);
    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    ASSERT_EQ(sum, 25);
}