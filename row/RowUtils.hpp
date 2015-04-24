//
// Created by john on 4/23/15.
//

#ifndef LMERGE_ROWUTILS_HPP
#define LMERGE_ROWUTILS_HPP

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

template<typename Str>
Str &operator<< (Str &str, const InputRow &row) {
  str << join (row, ",");
  return str;
}

#endif //LMERGE_ROWUTILS_HPP
