//
// Created by john on 4/17/15.
//

#ifndef LMERGE_TEXT_COLUMN_HPP
#define LMERGE_TEXT_COLUMN_HPP

#include "../std/memory.hpp"

#include "../column.hpp"
#include "../cell/text_cell.hpp"

class TextColumn
  :
    public InputColumn {
public:
  using InputColumn::InputColumn;

  ~TextColumn () {
  }

  std::unique_ptr<Cell> CreateCell (const std::string &data) const {
    return pstd::make_unique<TextCell> (data);
  }
};

#endif //LMERGE_TEXT_COLUMN_HPP
