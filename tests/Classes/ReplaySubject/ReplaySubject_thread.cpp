#include "rtk/ReplaySubject.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include <thread>

template <typename T>
void callReplaySubjectNext(rtk::ReplaySubject<T>* s, int nbr, T value)
{
    for (; nbr >= 0; nbr--)
        s->next(value);
}

TEST(replay_subject_thread, simple_add)
{
    rtk::ReplaySubject<int> s;
    int sum = 0;

    s.asObservable().subscribe([&sum](int value) {
        sum += value;
    });
    std::thread t(callReplaySubjectNext<int>, &s, 1, 1);
    t.join();
    ASSERT_EQ(sum, 2);
}

// TEST(replay_subject_thread, simple_print, .init = cr_redirect_stdout)
// {
//     rtk::ReplaySubject<std::string> s;
//     std::string str = "test";

//     s.asObservable().subscribe([](std::string value) {
//         std::cout << value << std::endl;
//     });
//     std::thread t(callReplaySubjectNext<std::string>, &s, 0, str);
//     t.join();
//     cr_assert_stdout_eq_str("test\n");
// }