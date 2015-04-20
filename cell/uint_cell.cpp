//
// Created by john on 4/17/15.
//

#include "../util/convert.hpp"
#include "uint_cell.hpp"

UIntCell::UIntCell (const std::string &data)
  :
  mData (Convert<uint64_t> (trim (data))) {

}

std::string UIntCell::Repr () const {
  std::stringstream ss;
  ss << mData;
  return ss.str ();
}

uint64_t UIntCell::Native () const {
  return mData;
}

bool UIntCell::operator== (const Cell &other) const {
  if (typeid (*this) != typeid (other)) {
    return false;
  }
  const UIntCell &that = static_cast <const UIntCell &> (other);
  return mData == that.mData;
}

bool UIntCell::operator== (Cell &other) {
  if (typeid (*this) != typeid (other)) {
    return false;
  }
  const UIntCell &that = static_cast <const UIntCell &> (other);
  return mData == that.mData;
}
