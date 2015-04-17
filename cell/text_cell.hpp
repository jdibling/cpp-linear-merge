//
// Created by john on 4/17/15.
//

#ifndef LMERGE_TEXT_CELL_H
#define LMERGE_TEXT_CELL_H

#include "../cell.hpp"

class TextCell
  :
    public Cell {
public:

  TextCell (const std::string &data)
    :
    mData (trim (data)) {
  }

  std::string Repr () const { return mData; }

  bool operator== (const Cell &other) const {
    if (typeid (*this) != typeid (other)) {
      return false;
    }
    const TextCell &that = static_cast <const TextCell &> (other);
    return mData == that.mData;
  }

private:
  std::string mData;
};

#endif //LMERGE_TEXT_CELL_H
