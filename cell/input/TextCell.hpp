//
// Created by john on 4/22/15.
//

#ifndef LMERGE_TEXTCELL_HPP
#define LMERGE_TEXTCELL_HPP

#include "../Cell.hpp"
#include "InputCell.hpp"

class TextCell
  :
    public InputCell {
public:
  using InputCell::InputCell;

  std::string Repr () const {
    return mOrigData;
  }

  const std::string &Val () const { return mOrigData; }

  bool operator== (const Cell &rhc) const {
    if (typeid (*this) != typeid (rhc)) {
      return false;
    }
    const TextCell &other = static_cast <const TextCell &> (rhc);
    return mOrigData == other.Orig ();
  }
};


#endif //LMERGE_TEXTCELL_HPP
