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
typedef std::unique_ptr<OutputRow> OutputRowPtr;

template<typename Str>
Str &operator<< (Str &str, const OutputRow &outRow) {
  for (OutputRow::const_iterator optCellPtrIt = outRow.begin ();
       optCellPtrIt != outRow.end ();
       ++optCellPtrIt) {
    const OptCellPtr &optCellPtr = *optCellPtrIt;
    if (optCellPtr) {
      CellPtr cellPtr = *optCellPtr;
      if (cellPtr) {
        const std::string &repr = cellPtr->Repr ();
        str << repr;
      }
    }
    str << ",";
  }

  return str;
}

#endif //LMERGE_OUTPUTROW_H
