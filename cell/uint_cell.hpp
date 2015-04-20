//
// Created by john on 4/17/15.
//

#ifndef LMERGE_UINT_CELL_HPP
#define LMERGE_UINT_CELL_HPP

#include "../cell.hpp"

class UIntCell
  :
    public Cell {
public:
  UIntCell (const std::string &data);

  std::string Repr () const;

  uint64_t Native () const;

  bool operator== (const Cell &other) const;

  bool operator== (Cell &other);

private:
  uint64_t mData;
};

#endif //LMERGE_UINT_CELL_HPP
