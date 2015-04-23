set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)

cmake_minimum_required(VERSION 3.0)
project(lmerge)

find_package (Boost)
include_directories (${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(lmerge main.cpp std/memory.hpp util/csv.hpp std/utility.hpp util/convert.hpp cell/Cell.cpp cell/Cell.hpp cell/CellFactory.cpp cell/CellFactory.hpp column/Column.cpp column/Column.hpp row/InputRow.cpp row/InputRow.hpp StepSearch.hpp cell/input/InputCell.cpp cell/input/InputCell.hpp cell/merge/MergeCell.cpp cell/merge/MergeCell.hpp cell/merge/DiffCell.cpp cell/merge/DiffCell.h row/OutputRowFactory.cpp row/OutputRowFactory.hpp row/OutputRow.cpp row/OutputRow.hpp row/RowUtils.hpp)
target_link_libraries (lmerge boost_filesystem boost_system)

