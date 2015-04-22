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
};

#include "Cell.hpp"

#endif //LMERGE_TEXTCELL_HPP
