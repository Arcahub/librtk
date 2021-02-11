#include "operator/syncAll.hpp"
#include "Subject.hpp"
#include <criterion/criterion.h>

Test(test_syncAll, empty)
{
    auto obs = rtk::syncAll<int>({});
    int sum = 0;

    obs.subscribe([&](auto) {
        sum += 5;
    });
    cr_assert_eq(sum, 0);
}

Test(test_syncAll, one)
{
    rtk::Subject<int> s;
    auto obs = rtk::syncAll<int>({ s.asObservable() });
    int sum = 0;

    obs.subscribe([&](auto values) {
        for (auto value : values)
            sum += value;
    });
    s.next(5);
    cr_assert_eq(sum, 5);
}

Test(test_syncAll, two)
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
    cr_assert_eq(sum, 0);
    s2.next(5);
    cr_assert_eq(sum, 10);
}

Test(test_syncAll, override)
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
    cr_assert_eq(sum, 0);
    s2.next(5);
    cr_assert_eq(sum, 10);
    s2.next(10);
    cr_assert_eq(sum, 30);
}