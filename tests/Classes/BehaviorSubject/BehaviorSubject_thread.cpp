#include "BehaviorSubject.hpp"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>
#include <string>
#include <thread>

template <typename T>
void callBehaviorSubjectNext(rtk::BehaviorSubject<T>* s, int nbr, T value)
{
    for (; nbr >= 0; nbr--)
        s->next(value);
}

Test(behavior_subject_thread, simple_add)
{
    rtk::BehaviorSubject<int> s(0);
    int sum = 0;

    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    std::thread t(callBehaviorSubjectNext<int>, &s, 1, 1);
    t.join();
    cr_assert_eq(sum, 2);
}

Test(behavior_subject_thread, simple_print, .init = cr_redirect_stdout)
{
    rtk::BehaviorSubject<std::string> s("");
    std::string str = "test";

    s.asObservable().subscribe([](std::string value) {
        std::cout << value << std::endl;
    });
    std::thread t(callBehaviorSubjectNext<std::string>, &s, 0, str);
    t.join();
    cr_assert_stdout_eq_str("\ntest\n");
}