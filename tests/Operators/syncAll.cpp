#include "rtk/operator/syncAll.hpp"
#include "rtk/Subject.hpp"
#include "gtest/gtest.h"

TEST(test_syncAll, empty)
{
    auto obs = rtk::syncAll<int>({});
    int sum = 0;

    obs.subscribe([&](auto) {
        sum += 5;
    });
    ASSERT_EQ(sum, 0);
}

TEST(test_syncAll, one)
{
    rtk::Subject<int> s;
    auto obs = rtk::syncAll<int>({ s.asObservable() });
    int sum = 0;

    obs.subscribe([&](auto values) {
        for (auto value : values)
            sum += value;
    });
    s.next(5);
    ASSERT_EQ(sum, 5);
}

TEST(test_syncAll, two)
{
    rtk::Subject<int> s1;
    rtk::Subject<int> s2;
    auto obs = rtk::syncAll<int>({ s1.asObservable(), s2.asObservable() });
    int sum = 0;

    obs.subscribe([&](auto values) {
        for (auto value : values)
            sum += value;
    });
    s1.next(5);
    ASSERT_EQ(sum, 0);
    s2.next(5);
    ASSERT_EQ(sum, 10);
}

TEST(test_syncAll, override)
{
    rtk::Subject<int> s1;
    rtk::Subject<int> s2;
    auto obs = rtk::syncAll<int>({ s1.asObservable(), s2.asObservable() });
    int sum = 0;

    obs.subscribe([&](auto values) {
        for (auto value : values)
            sum += value;
    });
    s1.next(5);
    s1.next(10);
    ASSERT_EQ(sum, 0);
    s2.next(5);
    ASSERT_EQ(sum, 10);
    s2.next(10);
    ASSERT_EQ(sum, 30);
}