//
// Created by john on 4/22/15.
//

#ifndef LMERGE_UINTCELL_HPP
#define LMERGE_UINTCELL_HPP

#include "Cell.hpp"

class UIntCell
  :
    public Cell {
public:
  UIntCell (const std::string &origData)
    :
    Cell (origData),
    mData (Convert<unsigned long> (origData)) {
  }

  std::string Repr () const {
    return Convert<std::string, unsigned long> (mData);
  }

private:
  unsigned long mData;
};

#endif //LMERGE_UINTCELL_HPP
