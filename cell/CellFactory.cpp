//
// Created by john on 4/22/15.
//

#include "CellFactory.hpp"
#include "../std/memory.hpp"

#include "input/TextCell.hpp"
#include "input/UIntCell.hpp"
#include "merge/DiffCell.h"

template<typename CellType, typename ...Args>
CellPtr MakeCell (const Args &... args) {
  return pstd::make_unique<CellType> (args...);
}

CellPtr CellFactory::CreateInputCell (const std::string &data) const {
  switch (mType) {
    case Type::Text: return MakeCell<TextCell> (data);
    case Type::UInt: return MakeCell<UIntCell> (data);
  }
}

CellPtr CellFactory::CreateMergeCell (InputRow const &left, InputRow const &right) const {
  switch (mType) {
    case Type::Diff: return MakeCell<DiffCell> (right[0], left[0]);
  }
}

