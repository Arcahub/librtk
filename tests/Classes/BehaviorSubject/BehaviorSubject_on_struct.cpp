#include "rtk/BehaviorSubject.hpp"
#include "rtk/ReplaySubject.hpp"
#include "gtest/gtest.h"

typedef struct Test_s {
    int val;
} Test_t;

TEST(test_behavior_subject_struct, create)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
}

TEST(test_behavior_subject_struct, next_without_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    Test_t v = { 1 };

    s.next(v);
}

TEST(test_behavior_subject_struct, error_without_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });

    s.error();
}

TEST(test_behavior_subject_struct, error_with_already_error_called)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    auto obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 0);
    s.error();
    ASSERT_EQ(sum.val, 5);
    s.error();
    ASSERT_EQ(sum.val, 5);
}

TEST(test_behavior_subject_struct, error_on_closed)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });

    s.unsubscribe();
    EXPECT_THROW(s.error(), std::exception);
}

TEST(test_behavior_subject_struct, complete_without_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });

    s.complete();
}

TEST(test_behavior_subject_struct, complete_with_already_complete_called)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    auto obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 0);
    s.complete();
    ASSERT_EQ(sum.val, 5);
    s.complete();
    ASSERT_EQ(sum.val, 5);
}

TEST(test_behavior_subject_struct, complete_on_closed)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });

    s.unsubscribe();
    EXPECT_THROW(s.complete(), std::exception);
}

TEST(test_behavior_subject_struct, asObservable)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
}

TEST(test_behavior_subject_struct, asObservable_two)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
}

TEST(test_behavior_subject_struct, subscribe_empty_one)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();

    obs.subscribe();
}

TEST(test_behavior_subject_struct, subscribe_empty_two)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();

    obs.subscribe();
    obs2.subscribe();
}

TEST(test_behavior_subject_struct, next_one_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    ASSERT_EQ(sum.val, 0);
    s.next({ 5 });
    ASSERT_EQ(sum.val, 5);
}

TEST(test_behavior_subject_struct, next_two_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    obs.subscribe(([&sum](Test_t value) mutable {
        sum.val += value.val;
    }));
    ASSERT_EQ(sum.val, 0);
    s.next({ 5 });
    ASSERT_EQ(sum.val, 10);
}

TEST(test_behavior_subject_struct, error_one_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 0);
    s.error();
    ASSERT_EQ(sum.val, 5);
}

TEST(test_behavior_subject_struct, error_two_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    obs.subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 0);
    s.error();
    ASSERT_EQ(sum.val, 10);
}

TEST(test_behavior_subject_struct, sub_on_already_error)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    Test_t sum = { 0 };

    s.error();
    s.asObservable().subscribe(nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 5);
}

TEST(test_behavior_subject_struct, sub_on_already_complete)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    Test_t sum = { 0 };

    s.complete();
    s.asObservable().subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 5);
}

TEST(test_behavior_subject_struct, complete_one_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    Test_t sum = { 0 };

    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 0);
    s.complete();
    ASSERT_EQ(sum.val, 5);
}

TEST(test_behavior_subject_struct, complete_two_sub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
    rtk::Observable<Test_t> obs = s.asObservable();
    rtk::Observable<Test_t> obs2 = s.asObservable();
    Test_t sum = { 0 };

    obs2.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    obs.subscribe(nullptr, nullptr, ([&sum]() mutable {
        sum.val += 5;
    }));
    ASSERT_EQ(sum.val, 0);
    s.complete();
    ASSERT_EQ(sum.val, 10);
}

TEST(test_behavior_subject_struct, unsub)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
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

TEST(test_behavior_subject_struct, unsub_expect_throw_on_next)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
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
    EXPECT_THROW(s.next({ 0 }), std::exception);
}

TEST(test_behavior_subject_struct, unsub_expect_throw_on_error)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
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
    EXPECT_THROW(s.error(), std::exception);
}

TEST(test_behavior_subject_struct, unsub_expect_throw_on_complete)
{
    rtk::BehaviorSubject<Test_t> s({ 0 });
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
    EXPECT_THROW(s.complete(), std::exception);
}

TEST(test_behavior_subject_struct, getValue_first)
{
    rtk::BehaviorSubject<Test_t> s({ 5 });

    ASSERT_EQ(s.getValue().val, 5);
}

TEST(test_behavior_subject_struct, getValue_after_next)
{
    rtk::BehaviorSubject<Test_t> s({ 5 });

    s.next({ 10 });
    ASSERT_EQ(s.getValue().val, 10);
}

TEST(test_behavior_subject_struct, replay)
{
    rtk::BehaviorSubject<Test_t> s({ 5 });
    int sum = 0;

    s.asObservable().subscribe([&sum](Test_t value) mutable {
        sum += value.val;
    });
    ASSERT_EQ(s.getValue().val, 5);
    ASSERT_EQ(sum, 5);
}

TEST(test_behavior_subject_struct, replay_with_next)
{
    rtk::BehaviorSubject<Test_t> s({ 5 });
    int sum = 0;

    s.asObservable().subscribe([&sum](Test_t value) mutable {
        sum += value.val;
    });
    s.next({ 10 });
    ASSERT_EQ(s.getValue().val, 10);
    ASSERT_EQ(sum, 15);
}
