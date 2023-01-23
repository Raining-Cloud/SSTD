cmake_minimum_required(VERSION 3.20)

set(PROJECTNAME "SSTDBench")

#include new tests here!
set(bench_sources ${bench_sources}
    DefaultBench.cpp
)

add_executable(${PROJECTNAME} ${bench_sources})
set_property(TARGET ${PROJECTNAME} PROPERTY CXX_STANDARD 20)
target_link_libraries(${PROJECTNAME} PUBLIC benchmark::benchmark SSTDLib)

target_include_directories(${PROJECTNAME} PUBLIC "${ENGINE_DEPENDENCIES_DIR}/SSTD/Source")