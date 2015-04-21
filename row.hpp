//
// Created by john on 4/17/15.
//

#ifndef LMERGE_INPUT_ROW_HPP
#define LMERGE_INPUT_ROW_HPP

#include <memory>
#include <vector>
#include <boost/iterator/indirect_iterator.hpp>
#include "std/memory.hpp"

#include "util/csv.hpp"

namespace csv = pico::util::csv;

#include "cell.hpp"
#include "column.hpp"

class Row {
  typedef std::unique_ptr<Cell> CellPtr;
  typedef std::vector<CellPtr> CellPtrs;
  CellPtrs mCells;

public:
  using iterator = boost::indirect_iterator<decltype (mCells)::iterator>;
  using const_iterator = boost::indirect_iterator<decltype (mCells)::const_iterator>;

  iterator begin () {
    return mCells.begin ();
  }

  const_iterator begin () const {
    return mCells.begin ();
  }

  iterator end () {
    return mCells.end ();
  }

  const_iterator end () const {
    return mCells.end ();
  }

  inline const Cell &operator[] (size_t idx) const {
    return *(mCells[idx]);
  }

  inline void push_back (CellPtr &&cell) {
    mCells.push_back (std::move (cell));
  }

private:
};

class RowFactory {
  const InputColumns &mCols;
public:
  //typedef std::unique_ptr <InputRow> RowPtr;

  RowFactory (const InputColumns &cols)
    :
    mCols (cols) {
  }

  Row Create (const csv::Row &csvRow) const {
    Row ret;

    for (size_t idx = 0; idx < csvRow.size (); ++idx) {
      const InputColumn &col = *mCols[idx];
      const std::string &cellData = csvRow[idx];
      ret.push_back (col.CreateCell (cellData));
    }

    return ret;
  }
};

//typedef std::vector<RowCell> InputRow;

typedef std::vector<Row> Rows;

#endif //LMERGE_INPUT_ROW_HPP
