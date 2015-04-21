//
// Created by john on 4/20/15.
//

#ifndef LMERGE_LATENCYCELL_H
#define LMERGE_LATENCYCELL_H

#include "../cell.hpp"
#include "../row.hpp"

class LatencyCell
  :
    public Cell {
public:
  LatencyCell (const Row &leftInput, const Row &rightInput);

  ~LatencyCell ();

  bool operator== (const Cell &rhs) const;

  std::string Repr () const;

  int64_t Native () const;

private:
  uint64_t mLeftRecv;
  uint64_t mRightRecv;
};


#endif //LMERGE_LATENCYCELL_H
