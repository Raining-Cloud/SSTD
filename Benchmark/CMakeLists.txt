cmake_minimum_required(VERSION 3.20)

set(PROJECTNAME "SSTDBench")

#include new tests here!
set(bench_sources ${bench_sources}
    main.cpp
    ToggleBenchmark.h
    DefaultBench.h
    VectorBench.h
    StringBench.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${bench_sources})
add_executable(${PROJECTNAME} ${bench_sources})
set_property(TARGET ${PROJECTNAME} PROPERTY CXX_STANDARD 20)
target_link_libraries(${PROJECTNAME} PUBLIC benchmark::benchmark SSTDLib)

target_include_directories(${PROJECTNAME} PRIVATE "../Source")