//
// Created by john on 4/17/15.
//

#include "uint_column.hpp"

UIntColumn::~UIntColumn () {

}

std::unique_ptr<Cell> UIntColumn::CreateCell (const std::string &data) const {
  return pstd::make_unique<UIntCell> (data);
}
