//
// Created by john on 4/22/15.
//

#include "CellFactory.hpp"
#include "../std/memory.hpp"

#include "TextCell.hpp"
#include "UIntCell.hpp"

template<typename CellType>
CellPtr MakeCell (const std::string &data) {
  return pstd::make_unique<CellType> (data);
}

CellPtr CellFactory::Create (const std::string &data) const {
  switch (mType) {
    case Type::Text: return MakeCell<TextCell> (data);
    case Type::UInt: return MakeCell<UIntCell> (data);

  }
}