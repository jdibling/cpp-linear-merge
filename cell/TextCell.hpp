//
// Created by john on 4/22/15.
//

#ifndef LMERGE_TEXTCELL_HPP
#define LMERGE_TEXTCELL_HPP

class TextCell
  :
    public Cell {
public:
  using Cell::Cell;

  std::string Repr () const {
    return mOrigData;
  }

  bool operator== (const Cell &rhc) const {
    if (typeid (*this) != typeid (rhc)) {
      return false;
    }
    const TextCell &other = static_cast <TextCell> (rhc);
    return mOrigData == rhc.Orig ();
  }
};

#include "Cell.hpp"

#endif //LMERGE_TEXTCELL_HPP
