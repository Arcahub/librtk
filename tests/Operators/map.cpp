#include "operator/map.hpp"
#include "Observable.hpp"
#include "Subject.hpp"
#include <criterion/criterion.h>

typedef struct Test_s {
    int val;
} Test_t;

Test(test_map, int_to_int)
{
    rtk::Subject<int> s;
    rtk::Observable<int> obs = (s.asObservable() >> rtk::map<int, int>([](int value) { return value; }));
    int sum = 0;

    obs.subscribe([&](int value) { sum = value; });
    s.next(5);
    cr_assert_eq(sum, 5);
}

Test(test_map, int_to_string)
{
    rtk::Subject<int> s;
    rtk::Observable<std::string> obs = (s.asObservable() >> rtk::map<int, std::string>([](int value) { return std::to_string(value); }));
    std::string res = "";

    obs.subscribe([&](std::string value) { res = value; });
    s.next(5);
    cr_assert_eq(res, "5");
}

Test(test_map, int_to_struct)
{
    rtk::Subject<int> s;
    rtk::Observable<Test_t> obs = (s.asObservable() >> rtk::map<int, Test_t>([](int value) { 
        Test_t res = {0};
        res.val = value;
        return res; }));
    Test_t res = { 0 };

    obs.subscribe([&](Test_t value) { res = value; });
    s.next(5);
    cr_assert_eq(res.val, 5);
}