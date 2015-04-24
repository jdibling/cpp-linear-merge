//
// Created by john on 4/22/15.
//

#ifndef LMERGE_INPUTCELL_H
#define LMERGE_INPUTCELL_H


#include "../Cell.hpp"

class InputCell
  :
    public Cell {
public:
  InputCell (const std::string &data);

  virtual std::string Orig () const;

protected:
  const std::string mOrigData;

};


#endif //LMERGE_INPUTCELL_H
