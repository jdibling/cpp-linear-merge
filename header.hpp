//
// Created by john on 4/13/15.
//
#include <iterator>
#include <algorithm>

#include <boost/iterator/indirect_iterator.hpp>

#include "std/memory.hpp"
#include "std/utility.hpp"
#include "util/convert.hpp"

#include "cell.hpp"
#include "column.hpp"
#include "input_row.hpp"

#ifndef QC_HEADER_HPP
#define QC_HEADER_HPP

template<typename Str>
inline Str &operator<< (Str &str, const Row &row) {
  std::stringstream ss;
  for (auto &cell: row) {
    ss << cell << ",";
  }

  str << ss.str ();
  return str;
}

bool Equ (const Row &lhs, const Row &rhs, const Columns &cols);

Columns GetInputFileHeader ();

#endif //QC_HEADER_HPP
