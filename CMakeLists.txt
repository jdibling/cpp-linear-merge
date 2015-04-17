set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)

cmake_minimum_required(VERSION 3.1)
project(lmerge)

find_package (Boost)
include_directories (${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(lmerge ${SOURCE_FILES} Options.cpp Options.h std/memory.hpp util/csv.hpp file/header.hpp std/utility.hpp)
target_link_libraries (lmerge boost_filesystem boost_system)