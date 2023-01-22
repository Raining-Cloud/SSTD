cmake_minimum_required(VERSION 3.20)

include(GoogleTest)

set(PROJECTNAME "SSTDTests")

#include new tests here!
set(test_sources ${test_sources}
    test_test.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_sources})
add_executable(${PROJECTNAME} ${test_sources})
target_link_libraries(${PROJECTNAME} GTest::gtest_main)
gtest_discover_tests(${PROJECTNAME})