cmake_minimum_required(VERSION 3.20)

include(GoogleTest)

set(PROJECTNAME "SSTDTests")

#include new tests here!
set(test_sources ${test_sources}
    DefaultTest.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${test_sources})
add_executable(${PROJECTNAME} ${test_sources})
set_property(TARGET ${PROJECTNAME} PROPERTY CXX_STANDARD 20)
target_link_libraries(${PROJECTNAME} PUBLIC GTest::gtest_main)

#don't forget to link the libary :D
target_link_libraries(${PROJECTNAME} PUBLIC SSTDLib)
target_include_directories(${PROJECTNAME} PRIVATE "../Source")

gtest_discover_tests(${PROJECTNAME})