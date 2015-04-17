//
// Created by john on 4/17/15.
//

#ifndef LMERGE_COLUMN_HPP
#define LMERGE_COLUMN_HPP

#include <string>
#include <memory>

#include "cell.hpp"

class Column {
public:
  enum class Importance {
    Unimportant,
    Key
  };

  Column (const std::string &name, Importance importance = Importance::Unimportant);

  virtual std::unique_ptr<Cell> CreateCell (const std::string &data) const = 0;

  virtual ~Column () = 0;

  const std::string mName;
  const Importance mImportance;
};

typedef std::vector<std::unique_ptr<Column>> Columns;


#endif //LMERGE_COLUMN_HPP
