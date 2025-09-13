cmake_minimum_required(VERSION 3.10)
project(CS106L
        DESCRIPTION "CS106L C++ Course"
        LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "10.0")
    message(STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  else()
    message(WARNING "!! We recommend that you use g++ version 10 or higher for this course. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, which may not support all C++23 features.")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message(STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} (Note: Course materials recommend g++)")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message(STATUS "You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} (Note: Course materials recommend g++)")
else()
  message(WARNING "!! We recommend that you use g++ for this course. You're using ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, which is not g++.")
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
add_executable(stringstreamExampleBug "05 - Streams/Code/stringstreamExampleBug.cpp")
add_executable(stringstreamExample "05 - Streams/Code/stringstreamExample.cpp")
add_executable(verifyPI "05 - Streams/Code/verifyPI.cpp")
add_executable(outputFileStreams "05 - Streams/Code/outputFileStreams.cpp")
add_executable(inputFileStreams "05 - Streams/Code/inputFileStreams.cpp")
add_executable(cinGetlineBug "05 - Streams/Code/cinGetlineBug.cpp")
add_executable(cinGetline "05 - Streams/Code/cinGetline.cpp")