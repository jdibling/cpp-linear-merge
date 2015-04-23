//
// Created by john on 4/23/15.
//

#ifndef LMERGE_ECHOCELL_H
#define LMERGE_ECHOCELL_H


#include "MergeCell.hpp"
#include "../../row/OutputRow.hpp"
#include "../../column/Column.hpp"

enum class Side {
  Left,
  Right
};

class EchoCell
  :
    public MergeCell {
  const std::string mColName;
  const Columns &mCols;
  const InputRow &mRow;
public:
  EchoCell (const std::string &colName, const Columns &cols, const InputRow &left, const InputRow &right)
    :
    mColName (colName),
    mCols (cols),
    mRow (side == Side::Left ? left : right) {
  }

  OptCellPtr GetCell () const {
    Columns::const_iterator colIt = std::find (mCols.begin (), mCols.end (), mColName);
    if (mCols.end () == colIt) {
      return OptCellPtr ();
    }
    else {
      return *colIt;
    }
  }

  std::string Repr () const {
    return "NotSupported";
  }

};


#endif //LMERGE_ECHOCELL_H
