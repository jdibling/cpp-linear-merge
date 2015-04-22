//
// Created by john on 4/22/15.
//

#ifndef LMERGE_CELL_H
#define LMERGE_CELL_H


#include <string>
#include <memory>

#include "../util/convert.hpp"

class Cell {
public:
  Cell (const std::string data)
    :
    mOrigData (data) {
  }

  virtual ~Cell () = 0;

  virtual std::string Repr () const = 0;

  std::string Orig () const { return mOrigData; }

protected:
  const std::string mOrigData;
};

typedef std::unique_ptr<Cell> CellPtr;


#endif //LMERGE_CELL_H
