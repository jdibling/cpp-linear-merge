//
// Created by john on 4/22/15.
//

#ifndef LMERGE_CELL_H
#define LMERGE_CELL_H


#include <string>
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

class TextCell
  :
    public Cell {
public:
  using Cell::Cell;

  std::string Repr () const {
    return mOrigData;
  }
};

class UIntCell
  :
    public Cell {
public:
  UIntCell (const std::string &origData)
    :
    Cell (origData),
    mData (Convert<uint64_t> (origData)) {
  }

  std::string Repr () const {
    return Convert<std::string, uint64_t> (mData);
  }

private:
  uint64_t mData;
};


#endif //LMERGE_CELL_H
