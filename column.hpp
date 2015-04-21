//
// Created by john on 4/17/15.
//

#ifndef LMERGE_COLUMN_HPP
#define LMERGE_COLUMN_HPP

#include <string>
#include <memory>

#include "cell.hpp"
#include "cell_factory.hpp"

class InputColumn {
public:
  enum class Importance {
    Unimportant,
    Key
  };

  InputColumn (InputCellFactory::Type type,
               const std::string &colName,
               Importance importance = Importance::Unimportant)
    :
    mFactory (type),
    mName (colame),
    mImportance (importance) {
  }

  const InputCellFactory mFactory;
  const std::string mName;
  const Importance mImportance;
};

typedef std::vector<std::unique_ptr<InputColumn>> InputColumns;


#endif //LMERGE_COLUMN_HPP
