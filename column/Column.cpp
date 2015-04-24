//
// Created by john on 4/22/15.
//

#include "Column.hpp"

Column::Column (const std::string &name, CellFactory cellFactory, Importance importance)
  :
  mName (name),
  mImportance (importance),
  mCellFactory (cellFactory) {
}

Column::~Column () {
}

CellPtr Column::CreateInputCell (const std::string &data) const {
  return mCellFactory.CreateInputCell (data);
}