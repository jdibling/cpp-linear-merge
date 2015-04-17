//
// Created by john on 4/17/15.
//

#ifndef LMERGE_INPUT_ROW_HPP
#define LMERGE_INPUT_ROW_HPP

#include <memory>
#include <vector>

#include "cell.hpp"

typedef std::unique_ptr<Cell> RowCell;
typedef std::vector<RowCell> InputRow;
typedef std::vector<InputRow> InputRows;


#endif //LMERGE_INPUT_ROW_HPP
