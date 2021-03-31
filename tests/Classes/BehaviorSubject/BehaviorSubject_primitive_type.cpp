#include "rtk/BehaviorSubject.hpp"
#include "gtest/gtest.h"

TEST(test_behavior_subject_primitive, create)
{
    rtk::BehaviorSubject<int> s(0);
}

TEST(test_behavior_subject_primitive, next_without_sub)
{
    rtk::BehaviorSubject<int> s(0);

    s.next(1);
}

TEST(test_behavior_subject_primitive, error_without_sub)
{
    rtk::BehaviorSubject<int> s(0);

    s.error();
}

TEST(test_behavior_subject_primitive, error_with_already_error_called)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, error_on_closed)
{
    rtk::BehaviorSubject<int> s(0);

    s.unsubscribe();
    EXPECT_THROW(s.error(), std::exception);
}

TEST(test_behavior_subject_primitive, complete_without_sub)
{
    rtk::BehaviorSubject<int> s(0);

    s.complete();
}

TEST(test_behavior_subject_primitive, complete_with_already_complete_called)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, complete_on_closed)
{
    rtk::BehaviorSubject<int> s(0);

    s.unsubscribe();
    EXPECT_THROW(s.complete(), std::exception);
}

TEST(test_behavior_subject_primitive, asObservable)
{
    rtk::BehaviorSubject<int> s(0);
    rtk::Observable<int> obs = s.asObservable();
}

TEST(test_behavior_subject_primitive, asObservable_two)
{
    rtk::BehaviorSubject<int> s(0);
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
}

TEST(test_behavior_subject_primitive, subscribe_empty_one)
{
    rtk::BehaviorSubject<int> s(0);
    rtk::Observable<int> obs = s.asObservable();

    obs.subscribe();
}

TEST(test_behavior_subject_primitive, subscribe_empty_two)
{
    rtk::BehaviorSubject<int> s(0);
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();

    obs.subscribe();
    obs2.subscribe();
}

TEST(test_behavior_subject_primitive, next_one_sub)
{
    rtk::BehaviorSubject<int> s(0);
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    ASSERT_EQ(sum, 0);
    s.next(5);
    ASSERT_EQ(sum, 5);
}

TEST(test_behavior_subject_primitive, next_two_sub)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, error_one_sub)
{
    rtk::BehaviorSubject<int> s(0);
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.error();
    ASSERT_EQ(sum, 5);
}

TEST(test_behavior_subject_primitive, error_two_sub)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, complete_one_sub)
{
    rtk::BehaviorSubject<int> s(0);
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.complete();
    ASSERT_EQ(sum, 5);
}

TEST(test_behavior_subject_primitive, complete_two_sub)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, unsub)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, unsub_expect_throw_on_next)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, unsub_expect_throw_on_error)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, unsub_expect_throw_on_complete)
{
    rtk::BehaviorSubject<int> s(0);
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

TEST(test_behavior_subject_primitive, getValue_first)
{
    rtk::BehaviorSubject<int> s(5);

    ASSERT_EQ(s.getValue(), 5);
}

TEST(test_behavior_subject_primitive, getValue_after_next)
{
    rtk::BehaviorSubject<int> s(5);

    s.next(10);
    ASSERT_EQ(s.getValue(), 10);
}

TEST(test_behavior_subject_primitive, replay)
{
    rtk::BehaviorSubject<int> s(5);
    int sum = 0;

    s.asObservable().subscribe([&sum](int value) mutable {
        sum += value;
    });
    ASSERT_EQ(s.getValue(), 5);
    ASSERT_EQ(sum, 5);
}

TEST(test_behavior_subject_primitive, replay_with_next)
{
    rtk::BehaviorSubject<int> s(5);
    int sum = 0;

    s.asObservable().subscribe([&sum](int value) mutable {
        sum += value;
    });
    s.next(10);
    ASSERT_EQ(s.getValue(), 10);
    ASSERT_EQ(sum, 15);
}
