//
// Created by john on 4/20/15.
//

#ifndef LMERGE_CELLFACTORY_H
#define LMERGE_CELLFACTORY_H


#include "input_row.hpp"

enum class CellType {
  Latency
};

class CellFactory {
public:
  CellFactory (CellType type)
    :
    mType (type) {

  }

  std::unique_ptr<Cell> CreateMergeCell (const InputRow &leftRow, const InputRow &rightRow);

private:
  CellType mType;
};


#endif //LMERGE_CELLFACTORY_H
