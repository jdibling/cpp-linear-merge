//
// Created by john on 4/22/15.
//

#include <string>
#include <sstream>

#include "RowUtils.hpp"

std::string InputRow::Repr () const {
  std::stringstream ss;
  ss << *this;
  return ss.str ();
}