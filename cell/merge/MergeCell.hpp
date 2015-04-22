//
// Created by john on 4/22/15.
//

#ifndef LMERGE_MERGECELL_H
#define LMERGE_MERGECELL_H

#include "../Cell.hpp"
#include "../../row/Row.hpp"

class MergeCell
  :
    public Cell {
public:
  enum class Side {
    Left,
    Right
  };

  MergeCell (const Cell &left, const Cell &right)
    :
    mLeft (left),
    mRight (right) {
  }

  inline const Cell &Orig (Side side) const {
    switch (side) {
      case Side::Left: return mLeft;
      case Side::Right: return mRight;
    }
  }

protected:
  const Cell &mLeft;
  const Cell &mRight;
};


#endif //LMERGE_MERGECELL_H
