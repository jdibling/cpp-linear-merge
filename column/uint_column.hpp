//
// Created by john on 4/17/15.
//

#ifndef LMERGE_UINT_COLUMN_HPP
#define LMERGE_UINT_COLUMN_HPP

#include "../std/memory.hpp"

#include "../column.hpp"
#include "../cell/uint_cell.hpp"

class UIntColumn
  :
    public InputColumn {
public:
  using InputColumn::InputColumn;

  ~UIntColumn ();

  std::unique_ptr<Cell> CreateCell (const std::string &data) const;
};

#endif //LMERGE_UINT_COLUMN_HPP
