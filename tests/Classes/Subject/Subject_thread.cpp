#include "Subject.hpp"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>
#include <string>
#include <thread>

template <typename T>
void callSubjectNext(rtk::Subject<T>* s, int nbr, T value)
{
    for (; nbr >= 0; nbr--)
        s->next(value);
}

Test(subject_thread, simple_add)
{
    rtk::Subject<int> s;
    int sum = 0;

    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    std::thread t(callSubjectNext<int>, &s, 1, 1);
    t.join();
    cr_assert_eq(sum, 2);
}

Test(subject_thread, simple_print, .init = cr_redirect_stdout)
{
    rtk::Subject<std::string> s;
    std::string str = "test";

    s.asObservable().subscribe([](std::string value) {
        std::cout << value << std::endl;
    });
    std::thread t(callSubjectNext<std::string>, &s, 0, str);
    t.join();
    cr_assert_stdout_eq_str("test\n");
}