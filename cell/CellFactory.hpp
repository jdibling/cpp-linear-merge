//
// Created by john on 4/22/15.
//

#ifndef LMERGE_CELLFACTORY_H
#define LMERGE_CELLFACTORY_H


#include "Cell.hpp"
#include "../row/InputRow.hpp"

class CellFactory {
public:
  enum class Type {
    // Simple (input) cells
      UInt,
    Text,

    // Computed (merge) cells
      Diff,
    Left,
    Right
  };

  CellFactory (Type type)
    :
    mType (type) {
  }

  CellPtr CreateInputCell (const std::string &data) const;

  CellPtr CreateMergeCell (InputRow const &left, InputRow const &right) const;

private:
  const Type mType;
};

template<CellFactory::Type FactoryType>
struct GenericFactory
  :
    public CellFactory {
public:
  GenericFactory ()
    :
    CellFactory (FactoryType) { }
};

typedef GenericFactory<CellFactory::Type::Text> Text;
typedef GenericFactory<CellFactory::Type::UInt> UInt;
typedef GenericFactory<CellFactory::Type::Diff> Diff;
typedef GenericFactory<CellFactory::Type::Left> Left;
typedef GenericFactory<CellFactory::Type::Right> Right;

#endif //LMERGE_CELLFACTORY_H
