//
// Created by john on 4/20/15.
//

#ifndef LMERGE_CELLFACTORY_H
#define LMERGE_CELLFACTORY_H


#include "row.hpp"

class InputCellFactory {
public:
  enum class Type {
    Text,
    UInt
  };

  InputCellFactory (Type type)
    :
    mType (type) {
  }

  std::unique_ptr<Cell> Create (const std::string &cellData) const;

private:
  Type mType;
};

enum class MergeCellType {
  Latency
};

class MergeCellFactory {
public:
  MergeCellFactory (MergeCellType type)
    :
    mType (type) {

  }

  std::unique_ptr<Cell> Create (const Row &leftRow, const Row &rightRow);

private:
  MergeCellType mType;
};


#endif //LMERGE_CELLFACTORY_H
