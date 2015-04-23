//
// Created by john on 4/23/15.
//

#ifndef LMERGE_OUTPUTROW_H
#define LMERGE_OUTPUTROW_H

#include <vector>
#include <boost/optional.hpp>

#include "../cell/Cell.hpp"

typedef boost::optional<CellPtr> OptCellPtr;
typedef std::vector<OptCellPtr> OptCellPtrs;
typedef OptCellPtrs OutputRow;

template<typename Str>
Str &operator<< (Str &str, const OutputRow &outRow) {

  for (const auto &cell: outRow) {
    if (cell) {
      str << (*cell)->Repr ();
    }
    str << ",";
  }

  return str;
}

#endif //LMERGE_OUTPUTROW_H
