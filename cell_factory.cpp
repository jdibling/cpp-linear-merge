//
// Created by john on 4/20/15.
//

#include <iostream>
#include "std/memory.hpp"

#include "cell_factory.hpp"
#include "cell/latency_cell.hpp"
#include "cell/text_cell.hpp"
#include "cell/uint_cell.hpp"

std::unique_ptr<Cell> InputCellFactory::Create (const std::string &cellData) const {
  switch (mType) {
    case Type::Text: return pstd::make_unique<TextCell> (cellData);
    case Type::UInt: return pstd::make_unique<UIntCell> (cellData);
    default :
      std::cerr << "Unhandled cell type: Creating Text cell instead." << std::endl;

  }
}

std::unique_ptr<Cell> MergeCellFactory::Create (const Row &leftRow, const Row &rightRow) {
  return pstd::make_unique<LatencyCell> (leftRow, rightRow);
}