//
// Created by john on 4/17/15.
//

#include "../cell.hpp"

Cell::~Cell () {

}

bool Cell::operator!= (const Cell &other) const {
  return !operator== (other);
}

