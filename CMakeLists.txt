set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)

cmake_minimum_required(VERSION 3.1)
project(lmerge)

find_package (Boost)
include_directories (${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(lmerge ${SOURCE_FILES} Options.cpp Options.h std/memory.hpp util/csv.hpp header.hpp std/utility.hpp mergerow.hpp util/convert.hpp cell.hpp cell/cell.cpp cell/uint_cell.hpp cell/uint_cell.cpp cell/text_cell.hpp cell/text_cell.cpp column.hpp column/text_column.cpp column/column.cpp column/text_column.hpp column/uint_column.hpp column/uint_column.cpp row.hpp row/row.cpp cell_factory.cpp cell_factory.hpp cell/latency_cell.cpp cell/latency_cell.hpp)
target_link_libraries (lmerge boost_filesystem boost_system)

