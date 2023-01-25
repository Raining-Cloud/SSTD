# SSTD (Snowing Standard/Template Library)
## Disclaimer
The SSTD is work in progress and is not intended for production use, I implement features as I need them. So it may contain bugs and lacks of features. Use at your own risk.
## Why?
There are several reasons why I want~~ed~~ to write my own standard template library:
- To **learn** about how the STL works internally: Writing an STL can be a great way to learn about the various   data structures and algorithms that are used in the STL, as well as how they are implemented.
- To **optimize** for specific use cases: While the STL is designed to be a general-purpose library, there may be specific scenarios where a custom implementation can be more efficient. For example, if I know that my data will always be sorted, I can write an STL implementation that takes advantage of this property to improve performance.
- To add **new features**: The STL is a large and comprehensive library, but it may not include every data structure or algorithm that I need. By writing my own STL, I can add any missing features that I need.
- To improve **compatibility** with other libraries: If I am using a library that has its own custom data structures and algorithms, I may want to write an STL implementation that is compatible with those data structures and algorithms to make it easier to use the two libraries together.
- As a **fun** programming project: Finally, writing an STL can simply be a fun and challenging programming project in and of itself. It can be a great way to improve my skills and deepen my understanding of C++.
## Features
- ### General
  - Sorting
  - Iterators (May get moved into the Container's)
  - Memory (TMemCpy, Bits and Bytes etc.)
  - Allocator as default Allocator-Stucture
  - Meta for templates/concepts etc.
  - Numeric (IntegralTypes and Limits)
  - Pattern (Idea of providing an Interface for Singelton and NonCopyable structures)
  - Utility (Move, Forward etc.)
- ### Containers
  - Array
  - Function
  - Pair
  - SmartPointer
  - Queue (STD Wrapper)
  - String
  - Vector
- ### Math
  - Vector
  - Matrices
- ### Platform
  - Threads
  - Mutex
  - Locks
  - ConditionVariables
  - Atomic (Integrals)
  - WindowAPI
  - Input
  - Shared-Libary Interface
## Documentation
For now there is none, working on it.
## Goals and Non-Goals
## Known-Issues
## Build
To use this library, you will need to have [CMake](https://cmake.org/) (Version 3.20 or higher) installed on your system. Then, you can use the following steps to build and install the SSTD:

### Manual
1. Clone the SSTD repository: `git clone --recurse-submodules https://github.com/cameronmcnz/surface.git `
2. Move into the SSTD directory: `cd SSTD`
3. Create a build directory: `mkdir build`
4. Move into the build directory: `cd build`
5. Run CMake: `cmake ..`

### Windows
1. Clone the SSTD repository: `git clone --recurse-submodules https://github.com/cameronmcnz/surface.git `
2. Run the `GenerateProjectFiles.bat` script.
## Licence
This library is licensed under the MIT License. Please see the [LICENSE](https://github.com/Raining-Cloud/SSTD/blob/main/LICENCE.txt) file for more details.
