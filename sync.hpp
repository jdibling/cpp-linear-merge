//
// Created by john on 4/17/15.
//

#ifndef LMERGE_SYNC_HPP
#define LMERGE_SYNC_HPP

#include <boost/optional.hpp>

#include "header.hpp"

typedef std::pair<InputRows::const_iterator, InputRows::const_iterator> Iters;
typedef boost::optional<Iters> SyncPoint;

inline SyncPoint FindSyncPoint (InputRows::const_iterator leftBegin,
                                InputRows::const_iterator leftEnd,
                                InputRows::const_iterator rightBegin,
                                InputRows::const_iterator rightEnd,
                                const Columns &columns) {
  for (auto it = leftBegin; it != leftEnd; ++it) {
//        const auto &leftSeq = (*it)[1]->Repr ();
//        const auto &rightSeq = rightInput.front ()[1]->Repr ();
    if (Equ (*it, *rightBegin, columns)) {
      return std::make_pair (it, rightBegin);
    }
  }

  // If we haven't found the sync point, iterate the right file looking for a sync point
  for (auto it = rightBegin; it != rightEnd; ++it) {
//        const auto &rightSeq = (*it)[1]->Repr ();
//        const auto &leftSeq = leftInput.front ()[1]->Repr ();
    if (Equ (*it, *leftBegin, columns)) {
      return std::make_pair (leftBegin, it);
    }
  }

  return boost::none;
}

#endif //LMERGE_SYNC_HPP
