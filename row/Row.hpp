//
// Created by john on 4/22/15.
//

#ifndef LMERGE_ROW_H
#define LMERGE_ROW_H

#include <vector>
#include <iterator>

#include <boost/algorithm/string/join.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/utility/result_of.hpp>

#include "../cell/Cell.hpp"

//typedef std::vector <CellPtr> Row;

class Row {
  std::vector<CellPtr> mCells;
public:
  typedef boost::indirect_iterator<decltype (mCells)::iterator> iterator;
  typedef boost::indirect_iterator<decltype (mCells)::const_iterator> const_iterator;
  typedef Cell &reference;
  typedef const Cell &const_reference;

  decltype (mCells)::size_type

  size () const { return mCells.size (); }
  iterator begin () { return mCells.begin (); }
  const_iterator begin () const { return mCells.begin (); }
  iterator end () { return mCells.end (); }
  const_iterator end () const { return mCells.end (); }

  const_reference front () const { return *begin (); }

  const Cell &operator[] (size_t idx) const { return *mCells[idx]; }

  template<typename Val>
  void push_back (const Val &val) { mCells.push_back (val); }
};

inline bool Equ (const Row &lhs, const Row &rhs, const Columns &columns) {
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

typedef std::vector<Row> Rows;

template<typename Cont>
inline std::string join (const Cont &cont, const std::string &delim) {
  typename Cont::const_iterator begin = std::begin (cont);
  typename Cont::const_iterator end = std::end (cont);

  std::string result;

  while (begin != end) {
    result.append (*begin++);
    if (begin != end) {
      result.append (delim);
    }
  }

  return result;

}

template<typename Str>
Str &operator<< (Str &str, const Row &row) {
  str << join (row, ",");
  return str;
}


#endif //LMERGE_ROW_H
