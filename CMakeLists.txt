cmake_minimum_required(VERSION 3.10)
project(CS106L
        DESCRIPTION "CS106L C++ Course"
        LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION MATCHES "^14.")
    message(STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  else()
    message(WARNING "!! We recommend that you use clang-14 for this course. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, a different version.")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message(STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
else()
  message(WARNING "!! We recommend that you use clang-14 for this course. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, which is not clang.")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiling lecture code executables
# 02 - Types and Structs
add_executable(types_and_structs "02 - Types and Structs/Code/main.cpp")

# 03 - Initialization and References
add_executable(const "03 - Initialization and References/Code/const.cpp")
add_executable(initialization "03 - Initialization and References/Code/initialization.cpp")
add_executable(reactor "03 - Initialization and References/Code/Reactor.cpp")
add_executable(references "03 - Initialization and References/Code/references.cpp")

# 05 - Streams
add_executable(stringstream_example "05 - Streams/Code/stringstreamExampleBug.cpp")


