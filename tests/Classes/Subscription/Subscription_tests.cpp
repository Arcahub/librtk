#include "rtk/Subject.hpp"
#include "gtest/gtest.h"

TEST(Subscription_test, simple_unsub_from_subject_obs)
{
    rtk::Subject<int> s;
    std::shared_ptr<rtk::Subscription> sub = s.asObservable().subscribe();

    sub->unsubscribe();
}

TEST(Subscription_test, unsub_after_next)
{
    rtk::Subject<int> s;
    int sum = 0;
    std::shared_ptr<rtk::Subscription> sub = s.asObservable().subscribe(
        [&sum](int value) {
            sum += value;
        });
    s.next(5);
    sub->unsubscribe();
    ASSERT_EQ(sum, 5);
}

TEST(Subscription_test, unsub_before_next)
{
    rtk::Subject<int> s;
    int sum = 0;
    std::shared_ptr<rtk::Subscription> sub = s.asObservable().subscribe(
        [&sum](int value) {
            sum += value;
        });
    sub->unsubscribe();
    s.next(5);
    ASSERT_EQ(sum, 0);
}

TEST(Subscription_test, unsub_first_of_two_next)
{
    rtk::Subject<int> s;
    int sum = 0;
    std::shared_ptr<rtk::Subscription> sub = s.asObservable().subscribe(
        [&sum](int value) {
            (void)value;
            sum += 5;
        });
    s.asObservable().subscribe(
        [&sum](int value) {
            (void)value;
            sum += 7;
        });
    sub->unsubscribe();
    s.next(5);
    ASSERT_EQ(sum, 7);
}

TEST(Subscription_test, unsub_second_of_two_next)
{
    rtk::Subject<int> s;
    int sum = 0;
    s.asObservable().subscribe(
        [&sum](int value) {
            (void)value;
            sum += 5;
        });
    std::shared_ptr<rtk::Subscription> sub = s.asObservable().subscribe(
        [&sum](int value) {
            (void)value;
            sum += 7;
        });
    sub->unsubscribe();
    s.next(5);
    ASSERT_EQ(sum, 5);
}

TEST(Subscription_test, double_unsub)
{
    rtk::Subject<int> s;
    int sum = 0;
    std::shared_ptr<rtk::Subscription> sub = s.asObservable().subscribe();
    sub->add([&sum]() {
        sum += 5;
    });
    sub->unsubscribe();
    sub->unsubscribe();
    ASSERT_EQ(sum, 5);
}

TEST(Subscription_test, add_teardow_on_closed)
{
    int sum = 0;
    std::shared_ptr<rtk::Subscription> sub = rtk::Subscription::empty();
    sub->add([&sum]() {
        sum += 5;
    });
    ASSERT_EQ(sum, 5);
}

TEST(Subscription_test, add_Subscription_on_closed)
{
    int sum = 0;
    std::shared_ptr<rtk::Subscription> sub = rtk::Subscription::empty();
    std::shared_ptr<rtk::Subscription> sub2(new rtk::Subscription([&sum]() { sum += 5; }));
    sub->add(sub2);
    ASSERT_EQ(sum, 5);
}

TEST(Subscription_test, add_same_Subscription_on_closed)
{
    int sum = 0;
    std::shared_ptr<rtk::Subscription> sub(new rtk::Subscription([&sum]() { sum += 5; }));
    sub->add(sub);
    ASSERT_EQ(sum, 0);
}