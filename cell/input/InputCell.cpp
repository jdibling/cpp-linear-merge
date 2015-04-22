//
// Created by john on 4/22/15.
//

#include "InputCell.hpp"


InputCell::InputCell (const std::string &data)
  :
  mOrigData (data) {
}

std::string InputCell::Orig () const {
  return mOrigData;
}