//
// Created by john on 4/22/15.
//

#ifndef LMERGE_DIFFCELL_H
#define LMERGE_DIFFCELL_H

#include "../input/UIntCell.hpp"

#include "MergeCell.hpp"

class DiffCell
  :
    public MergeCell {
public:
  using MergeCell::MergeCell;

  std::string Repr () const {
    return Convert<std::string> (Diff ());
  }

  uint64_t Diff () const {
    const UIntCell &left = static_cast <const UIntCell &> (Orig (Side::Left));
    const UIntCell &right = static_cast <const UIntCell &> (Orig (Side::Right));
    return left.Val () - right.Val ();
  }

  bool operator== (const Cell &) const {
    // comparison of merge cells not supported
    return false;
  }

};


#endif //LMERGE_DIFFCELL_H
