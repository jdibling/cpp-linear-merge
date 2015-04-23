//
// Created by john on 4/22/15.
//

#ifndef LMERGE_DIFFCELL_H
#define LMERGE_DIFFCELL_H

#include "../input/UIntCell.hpp"

#include "MergeCell.hpp"
#include "../../row/OutputRow.hpp"
#include "EchoCell.h"

class DiffCell
  :
    public MergeCell {
  const EchoCell &mLeft;
  const EchoCell &mRight;

public:
  DiffCell (const EchoCell &left, const EchoCell &right)
    :
    mLeft (left),
    mRight (right) {
  }


  uint64_t Diff () const {
    const UIntCell &left = static_cast <const UIntCell &> (mLeft);
    const UIntCell &right = static_cast <const UIntCell &> (mRight);
    return left.Val () - right.Val ();
  }

  std::string Repr () const {
    return Convert<std::string> (Diff ());
  }

  bool operator== (const Cell &) const {
    // comparison of merge cells not supported
    return false;
  }

};


#endif //LMERGE_DIFFCELL_H
