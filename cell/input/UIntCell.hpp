//
// Created by john on 4/22/15.
//

#ifndef LMERGE_UINTCELL_HPP
#define LMERGE_UINTCELL_HPP

#include "../Cell.hpp"
#include "InputCell.hpp"

class UIntCell
  :
    public InputCell {
public:
  explicit UIntCell (const std::string &origData)
    :
    InputCell (origData),
    mData (Convert<uint64_t> (origData)) {
  }

  std::string Repr () const {
    return Convert<std::string> (mData);
  }

  uint64_t Val () const { return mData; }

  inline bool operator== (const Cell &rhc) const {
    if (typeid (*this) != typeid (rhc)) {
      return false;
    }
    const UIntCell &other = static_cast <const UIntCell &> (rhc);
    return mData == other.mData;
  }

private:
  unsigned long mData;
};

#endif //LMERGE_UINTCELL_HPP
