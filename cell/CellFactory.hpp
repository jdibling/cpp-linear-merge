//
// Created by john on 4/22/15.
//

#ifndef LMERGE_CELLFACTORY_H
#define LMERGE_CELLFACTORY_H


#include "Cell.hpp"

class CellFactory {
public:
  enum class Type {
    // Simple (input) cells
      UInt,
    Text
  };

  CellFactory (Type type)
    :
    mType (type) {
  }

  CellPtr Create (const std::string &data) const;

private:
  const Type mType;
};

typedef CellFactory::Type CellType;


#endif //LMERGE_CELLFACTORY_H
