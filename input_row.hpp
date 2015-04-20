//
// Created by john on 4/17/15.
//

#ifndef LMERGE_INPUT_ROW_HPP
#define LMERGE_INPUT_ROW_HPP

#include <memory>
#include <vector>
#include <boost/iterator/indirect_iterator.hpp>

#include "cell.hpp"
#include "column.hpp"

class InputRow {
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

//typedef std::vector<RowCell> InputRow;

class InputRows {
  typedef std::vector<InputRow> Rows;
  const Columns &mCols;
  Rows mRows;
public:
  typedef Rows::iterator iterator;
  typedef Rows::const_iterator const_iterator;

  InputRows (const Columns &cols)
    :
    mCols (cols) {
  }

  const Columns &GetColumns () const {
    return mCols;
  }

  template<typename... Args>
  inline void emplace_back (Args &&... args) {
    mRows.emplace_back (args...);
  }

  const Rows::const_iterator begin () const { return mRows.begin (); }

  Rows::iterator begin () { return mRows.begin (); }

  const Rows::const_iterator end () const { return mRows.end (); }

  Rows::iterator end () { return mRows.end (); }

  const Rows::reference back () { return mRows.back (); }

  Rows::size_type size () const { return mRows.size (); }
};

#endif //LMERGE_INPUT_ROW_HPP
