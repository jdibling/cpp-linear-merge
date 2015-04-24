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
    Select
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
//typedef GenericFactory<CellFactory::Type::Diff> Diff;

enum class Side {
  Left,
  Right
};

class Select
  :
    public GenericFactory<CellFactory::Type::Select> {
  const std::string mColName;
  const Side mSide;
public:
  Select (const std::string &colName, Side side)
    :
    mColName (colName),
    mSide (side) {
  }
};

class Left
  :
    public Select {
public:
  Left (const std::string &colName)
    :
    Select (colName, Side::Left) {
  }
};

class Right
  :
    public Select {
public:
  Right (const std::string &colName)
    :
    Select (colName, Side::Right) {
  }
};

class Diff
  :
    public GenericFactory<CellFactory::Type::Diff> {
  const Select &mFirst;
  const Select &mSecond;
public:
  Diff (const Select &first, const Select &second)
    :
    mFirst (first),
    mSecond (second) {
  }
};


#endif //LMERGE_CELLFACTORY_H
