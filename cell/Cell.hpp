//
// Created by john on 4/22/15.
//

#ifndef LMERGE_CELL_H
#define LMERGE_CELL_H


#include <string>
#include <memory>
#include <iostream>

#include "../util/convert.hpp"

class Cell {
public:
  virtual ~Cell () = 0;

  virtual std::string Repr () const = 0;
  operator std::string () const { return Repr (); }

  virtual bool operator== (const Cell &rhc) const = 0;

  virtual bool operator!= (const Cell &rhc) const {
    return !operator== (rhc);
  }
};

typedef std::shared_ptr<Cell> CellPtr;



#endif //LMERGE_CELL_H
