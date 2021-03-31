cmake_minimum_required(VERSION 3.8)

if(TARGET rtk)
    return()
endif()

project(rtk)
include(CTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(sources
    "include/rtk/Observable.hpp"
)

add_library(rtk INTERFACE)

target_include_directories(rtk INTERFACE "include")

option(BUILD_TESTING "Build the tests" ON)

if(BUILD_TESTING)
    enable_testing()
    include(GoogleTest)
    add_subdirectory("extern/googletest")
    

    add_executable(rtk_unit_tests
            "tests/Classes/BehaviorSubject/BehaviorSubject_on_struct.cpp"
            "tests/Classes/BehaviorSubject/BehaviorSubject_primitive_type.cpp"
            "tests/Classes/BehaviorSubject/BehaviorSubject_thread.cpp"
            "tests/Classes/Observable/Observable.cpp"
            "tests/Classes/ReplaySubject/ReplaySubject_on_struct.cpp"
            "tests/Classes/ReplaySubject/ReplaySubject_primitive_type.cpp"
            "tests/Classes/ReplaySubject/ReplaySubject_thread.cpp"
            "tests/Classes/Subject/Subject_on_struct.cpp"
            "tests/Classes/Subject/Subject_primitive_type.cpp"
            "tests/Classes/Subject/Subject_thread.cpp"
            "tests/Classes/Subscription/Subscription_tests.cpp"
            "tests/Operators/syncAll.cpp"
    )

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(rtk_unit_tests rtk gtest gmock gtest_main Threads::Threads)

endif()
