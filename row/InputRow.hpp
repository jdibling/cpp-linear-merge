//
// Created by john on 4/22/15.
//

#ifndef LMERGE_ROW_H
#define LMERGE_ROW_H

#include <vector>
#include <iterator>

#include "../std/utility.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/utility/result_of.hpp>

#include "../cell/Cell.hpp"
#include "../column/Column.hpp"

//typedef std::vector <CellPtr> Row;

class InputRow {
  std::vector<CellPtr> mCells;
public:
  typedef boost::indirect_iterator<decltype (mCells)::iterator> iterator;
  typedef boost::indirect_iterator<decltype (mCells)::const_iterator> const_iterator;
  typedef decltype (mCells)
  ::iterator piterator;
  typedef decltype (mCells)
  ::const_iterator const_piterator;
  typedef Cell &reference;
  typedef const Cell &const_reference;

  decltype (mCells)::size_type

  size () const { return mCells.size (); }
  iterator begin () { return mCells.begin (); }
  const_iterator begin () const { return mCells.begin (); }

  piterator pbegin () { return mCells.begin (); }

  const_piterator pbegin () const { return mCells.begin (); }
  iterator end () { return mCells.end (); }
  const_iterator end () const { return mCells.end (); }

  piterator pend () { return mCells.end (); }

  const_piterator pend () const { return mCells.end (); }

  const_reference front () const { return *begin (); }

  const Cell &operator[] (size_t idx) const { return *mCells[idx]; }

  template<typename Val>
  void push_back (const Val &val) { mCells.push_back (val); }
};

inline bool Equ (const InputRow &lhs, const InputRow &rhs, const Columns &columns) {
  // assumption: lhs, rhs and columns all have the same number of elements
  for (size_t idx = 0; idx != lhs.size (); ++idx) {
    const Cell &lhc = lhs[idx];
    const Cell &rhc = rhs[idx];
    const Column &col = columns[idx];

    if (col.mImportance == Column::Importance::Key) {
      if (lhc != rhc) {
        return false;
      }
    }
  }

  return true;
}

typedef std::vector<InputRow> Rows;

template<typename Str>
Str &operator<< (Str &str, const InputRow &row) {
  str << join (row, ",");
  return str;
}


#endif //LMERGE_ROW_H
