//
// Created by john on 4/20/15.
//

#include <sstream>
#include "latency_cell.hpp"
#include "uint_cell.hpp"

LatencyCell::LatencyCell (const InputRow &leftInput, const InputRow &rightInput)
  :
  mLeftRecv (static_cast <const UIntCell &> (leftInput[0]).Native ()),
  mRightRecv (static_cast <const UIntCell &> (rightInput[0]).Native ()) {
}

LatencyCell::~LatencyCell () {
}

int64_t LatencyCell::Native () const {
  if (mLeftRecv <= mRightRecv) {
    return static_cast <int64_t> (mRightRecv - mLeftRecv);
  }
  else {
    return -static_cast<int64_t> (mLeftRecv - mRightRecv);
  }
}

std::string LatencyCell::Repr () const {
  std::stringstream ss;
  ss << Native ();
  return ss.str ();
}

bool LatencyCell::operator== (const Cell &other) const {
  if (typeid (*this) != typeid (other)) {
    return false;
  }
  const LatencyCell &that = static_cast <const LatencyCell &> (other);
  return (mRightRecv == that.mRightRecv) && (mLeftRecv == that.mLeftRecv);

}