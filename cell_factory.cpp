//
// Created by john on 4/20/15.
//

#include "std/memory.hpp"

#include "cell_factory.hpp"
#include "cell/latency_cell.hpp"

std::unique_ptr<Cell> CellFactory::CreateMergeCell (const InputRow &leftRow, const InputRow &rightRow) {
  return pstd::make_unique<LatencyCell> (leftRow, rightRow);
}