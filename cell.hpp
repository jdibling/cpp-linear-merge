//
// Created by john on 4/17/15.
//

#ifndef LMERGE_CELL_HPP
#define LMERGE_CELL_HPP

#include <string>
#include <memory>
#include "std/utility.hpp"

class Cell {
public:
  virtual ~Cell () = 0;

  virtual std::string Repr () const = 0;

  virtual bool operator== (const Cell &other) const = 0;

  virtual bool operator!= (const Cell &other) const; // default calls negative of op==
};

typedef std::unique_ptr<Cell> CellPtr;

template<typename Str>
inline Str &operator<< (Str &str, const Cell &cell) {
  str << cell.Repr ();
  return str;
}


#endif //LMERGE_CELL_HPP
