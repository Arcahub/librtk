#include "rtk/Subject.hpp"
#include "gtest/gtest.h"

TEST(test_subject_primitive, create)
{
    rtk::Subject<int> s;
}

TEST(test_subject_primitive, next_without_sub)
{
    rtk::Subject<int> s;

    s.next(1);
}

TEST(test_subject_primitive, error_without_sub)
{
    rtk::Subject<int> s;

    s.error();
}

TEST(test_subject_primitive, error_with_already_error_called)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, error_on_closed)
{
    rtk::Subject<int> s;

    s.unsubscribe();
    EXPECT_THROW(s.error(), std::exception);
}

TEST(test_subject_primitive, complete_without_sub)
{
    rtk::Subject<int> s;

    s.complete();
}

TEST(test_subject_primitive, complete_with_already_complete_called)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, complete_on_closed)
{
    rtk::Subject<int> s;

    s.unsubscribe();
    EXPECT_THROW(s.complete(), std::exception);
}

TEST(test_subject_primitive, asObservable)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = s.asObservable();
}

TEST(test_subject_primitive, asObservable_two)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();
}

TEST(test_subject_primitive, subscribe_empty_one)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = s.asObservable();

    obs.subscribe();
}

TEST(test_subject_primitive, subscribe_empty_two)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    rtk::Observable<int> obs2 = s.asObservable();

    obs.subscribe();
    obs2.subscribe();
}

TEST(test_subject_primitive, next_one_sub)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(([&sum](int value) mutable {
        sum += value;
    }));
    ASSERT_EQ(sum, 0);
    s.next(5);
    ASSERT_EQ(sum, 5);
}

TEST(test_subject_primitive, next_two_sub)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, error_one_sub)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.error();
    ASSERT_EQ(sum, 5);
}

TEST(test_subject_primitive, error_two_sub)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, complete_one_sub)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = s.asObservable();
    int sum = 0;

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum += 5;
    }));
    ASSERT_EQ(sum, 0);
    s.complete();
    ASSERT_EQ(sum, 5);
}

TEST(test_subject_primitive, complete_two_sub)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, unsub)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, unsub_expect_throw_on_next)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, unsub_expect_throw_on_error)
{
    rtk::Subject<int> s;
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

TEST(test_subject_primitive, unsub_expect_throw_on_complete)
{
    rtk::Subject<int> s;
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