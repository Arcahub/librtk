cmake_minimum_required(VERSION 3.8)

if(TARGET rtk)
    return()
endif()

project(rtk)
include(CTest)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(sources
    "include/rtk/Observable.hpp"
)

add_library(rtk INTERFACE)

target_include_directories(rtk INTERFACE "include")

if (BUILD_TESTING)
    find_package(Criterion)

    if(Criterion_FOUND)

        set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)

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
        target_link_libraries(rtk_unit_tests PRIVATE rtk)
        target_link_libraries(rtk_unit_tests PRIVATE ${CRITERION_LIBRARIES})
        target_include_directories(rtk_unit_tests
            PRIVATE ${CRITERION_INCLUDE_DIRS})

        target_link_libraries(rtk_unit_tests PRIVATE Threads::Threads)

        add_test(
            NAME rtk_tests_build
            COMMAND "${CMAKE_COMMAND}"
                    --build "${CMAKE_BINARY_DIR}"
                    --config "$<CONFIG>"
                    --target rtk_unit_tests
        )
        set_tests_properties(rtk_tests_build
            PROPERTIES FIXTURES_SETUP rtk_tests_fixture)

        add_test(NAME rtk_tests COMMAND rtk_unit_tests)
        set_tests_properties(rtk_tests
            PROPERTIES FIXTURES_REQUIRED rtk_tests_fixture)
    endif()
endif()