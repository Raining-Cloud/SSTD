cmake_minimum_required(VERSION 3.20)

set(PROJECTNAME "SSTDLib")
set(SSTD_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/")

set(general_sources ${general_sources}
   General/Algorithm.h
   General/Allocator.h
   General/Meta.h 
   General/Iterator.h
   General/Memory.h
   General/Numeric.h
   General/Pattern.h
   General/Utility.h
   General/Exception.h
   General/Error.h
   General/Result.h
)

set(container_sources ${container_sources}
   Containers/Array.h
   Containers/Pair.h
   Containers/Queue.h
   Containers/Vector.h
   Containers/Rect.h
   Containers/String.h
   Containers/Color.h
   Containers/Pointer.h
   Containers/Function.h
)

set(platform_sources ${platform_sources}
   Platform/IncludePlatform.h
   Platform/DefinePlatform.h

   Platform/PlatformWrapper/WindowsPlatform.h
   Platform/PlatformWrapper/WindowsPlatform.cpp

   Platform/IO/InputDeclarations.h
   Platform/IO/File.h
   Platform/IO/File.cpp
   Platform/IO/SharedLibrary.h
   Platform/IO/SharedLibrary.cpp

   Platform/Window/Window.h
   Platform/Window/Window.cpp
   Platform/Window/WindowEvent.h
   Platform/Window/SplashScreen.h
   Platform/Window/SplashScreen.cpp

   Platform/Console/Console.h
   Platform/Console/Console.cpp

   Platform/Threading/Thread.h
   Platform/Threading/Thread.cpp
   Platform/Threading/Mutex.h
   Platform/Threading/Mutex.cpp
   Platform/Threading/ConditionVariable.h
   Platform/Threading/ConditionVariable.cpp
   Platform/Threading/Lock.h
   Platform/Threading/AtomicUtils.h
   Platform/Threading/Atomic.h
   Platform/Threading/Atomic.cpp
)

set(math_sources ${math_sources}
    Math/Math.h
    Math/Vec.h
    Math/Mat.h
)

set(experimental_sources ${experimental_sources}
   Experimental/List.h
   Experimental/SIMD.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${general_sources} ${container_sources} ${math_sources} ${platform_sources} ${experimental_sources})
add_library(${PROJECTNAME} STATIC ${general_sources} ${container_sources} ${math_sources} ${platform_sources} ${experimental_sources})

set_property(TARGET ${PROJECTNAME} PROPERTY LINKER_LANGUAGE CXX)
set_property(TARGET ${PROJECTNAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECTNAME} PRIVATE ${SSTD_INCLUDE})
target_link_libraries(${PROJECTNAME} INTERFACE "dwmapi.lib")