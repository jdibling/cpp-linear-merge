//
// Created by john on 4/22/15.
//

#ifndef LMERGE_STEPSEARCH_HPP
#define LMERGE_STEPSEARCH_HPP

#include "row/InputRow.hpp"
#include "row/RowUtils.hpp"
#include "column/Column.hpp"

enum class SearchSide {
  Left,
  Right,
  Neither, // sides matched from the beginning
  Both,    // no match found -- remaining rows on both sides are orphans
};

// a structure to return from the step-search algo
struct StepSearchResults {
  Rows::const_iterator mLeftIt;
  Rows::const_iterator mRightIt;
  SearchSide mWhichAdvanced;

  StepSearchResults (Rows::const_iterator leftIt, Rows::const_iterator rightIt, SearchSide side)
    :
    mLeftIt (leftIt),
    mRightIt (rightIt),
    mWhichAdvanced (side) {
  }
};

/***
 * StepSearch algorithm
 */

StepSearchResults StepSearch (const Rows::const_iterator leftBegin,
                              const Rows::const_iterator leftEnd,
                              const Rows::const_iterator rightBegin,
                              const Rows::const_iterator rightEnd,
                              const Columns &columns,
                              bool isFirstSync) {
  // initial test for equality
  if (Equ (*leftBegin, *rightBegin, columns)) {
    // match found at beginning
    return StepSearchResults (leftBegin, rightBegin, SearchSide::Neither);
  }

  // the rows we're testing for equality
  Rows::const_iterator leftIt = leftBegin;
  Rows::const_iterator rightIt = rightBegin;

  const InputRow &leftAnchor = *leftBegin;
  const InputRow &rightAnchor = *rightBegin;

  // continue stepping until we reach the last element on both sides
  const Rows::const_iterator leftLastIt = std::prev (leftEnd);
  const Rows::const_iterator rightLastIt = std::prev (rightEnd);
  while (!(leftIt == leftLastIt && rightIt == rightLastIt)) {
    const InputRow &leftRow = *leftIt;
    const InputRow &rightRow = *rightIt;

    // if we aren't looking for the *first* sync point,
    // check to see if we found a match by advancing both
    // iterators
    if (!isFirstSync) {
      if (Equ (leftRow, rightRow, columns)) {
        return StepSearchResults (leftIt, rightIt, SearchSide::Both);
      }
    }

    // iterators pointing to the last element on each side
    const bool leftAnchorIsLast = (leftBegin == leftLastIt);
    const bool leftItIsLast = (leftIt == leftLastIt);
    const bool rightAnchorIsLast = (rightBegin == rightLastIt);
    const bool rightItIsLast = (rightIt == rightLastIt);

    // advance the left and test against the right anchor
    if (!leftAnchorIsLast && !leftItIsLast) {
      leftIt = std::next (leftIt);
      if (Equ (*leftIt, rightAnchor, columns)) {
        // we found a match by advanccing the left side
        return StepSearchResults (leftIt, rightBegin, SearchSide::Left);
      }
    }

    // now advance the right and test against the left anchor
    if (!rightAnchorIsLast && !rightItIsLast) {
      rightIt = std::next (rightIt);
      if (Equ (leftAnchor, *rightIt, columns)) {
        // we found a match by advancing the right side
        return StepSearchResults (leftBegin, rightIt, SearchSide::Right);
      }
    }
  }

  // if we have gotten here, then we have reached the end of both files
  // without finding a match.  We report that both sides were advanced.
  return StepSearchResults (leftIt, rightIt, SearchSide::Both);
};

#endif //LMERGE_STEPSEARCH_HPP
