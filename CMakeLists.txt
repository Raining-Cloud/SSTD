cmake_minimum_required(VERSION 3.20)

project(SSTD VERSION 1.0.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Output/Lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Output/Bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Output/Bin")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(Dependencies)
add_subdirectory(Source)
add_subdirectory(Testing)
add_subdirectory(Benchmark)