//
// Created by john on 4/22/15.
//

#ifndef LMERGE_COLUMN_H
#define LMERGE_COLUMN_H

#include <vector>

#include "../cell/CellFactory.hpp"

class Column {
public:
  enum class Importance {
    Key,
    Unimportant,
    Output
  };

  Column (const std::string &name, CellFactory::Type type,
          Importance importance = Importance::Unimportant);  // constructor for input input cell column
  ~Column ();

  CellPtr CreateInputCell (const std::string &data) const;

  const std::string mName;
  const Importance mImportance;
  const CellFactory mCellFactory;
};

typedef Column::Importance Importance;

typedef std::vector<Column> Columns;

#endif //LMERGE_COLUMN_H
