cmake_minimum_required(VERSION 3.20)

# add the GoogleTest Framework as seen here https://google.github.io/googletest/quickstart-cmake.html
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)

if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)
include(GoogleTest)
set_target_properties(gtest PROPERTIES FOLDER Google/GoogleTest)
set_target_properties(gtest_main PROPERTIES FOLDER Google/GoogleTest)
set_target_properties(gmock PROPERTIES FOLDER Google/GoogleTest)
set_target_properties(gmock_main PROPERTIES FOLDER Google/GoogleTest)


set(PROJECTNAME "SSTDTests")

#include new tests here!
set(test_sources ${test_sources}
    test_test.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_sources})
add_executable(${PROJECTNAME} ${test_sources})
target_link_libraries(${PROJECTNAME} GTest::gtest_main)
gtest_discover_tests(${PROJECTNAME})