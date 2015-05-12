//
// Created by john on 4/22/15.
//

#ifndef LMERGE_STEPSEARCH_HPP
#define LMERGE_STEPSEARCH_HPP

#include "row/InputRow.hpp"
#include "row/RowUtils.hpp"
#include "column/Column.hpp"

static const std::string MatchingAlgoVersion = "StepSearch 2.1";

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

Rows::const_iterator SearchOneSide (const InputRow& staticRow, 
                                    Rows::const_iterator movingBegin,
                                    Rows::const_iterator movingEnd,
                                    const Columns& columns)
{
  // look for a match by advancing the moving iterator and comparing to the static row
  for (Rows::const_iterator currentRowIt = movingBegin;
      currentRowIt != movingEnd;
      ++currentRowIt)
  {
    const InputRow& currentRow = *currentRowIt;
    if (Equ (staticRow, currentRow, columns))
        return currentRowIt;
  }
  return movingEnd;
}

StepSearchResults StepSearch (const Rows::const_iterator leftBegin,
                              const Rows::const_iterator leftEnd,
                              const Rows::const_iterator rightBegin,
                              const Rows::const_iterator rightEnd,
                              const Columns &columns,
                              bool isFirstSync)
{
  // DEBUG
  const std::string firstLeftSeq = (leftBegin != leftEnd) ? (*leftBegin)[1].Repr() : "<end>";
  const std::string firstRightSeq = (rightBegin != rightEnd) ? (*rightBegin)[1].Repr() : "<end>";
  // \DEBUG
  
  // quick test for equality (normal case)
  if (Equ (*leftBegin, *rightBegin, columns))
  {
    return StepSearchResults (leftBegin, rightBegin, SearchSide::Neither);
  }

  // advance the left iterator to see if we can match the right begin
  const Rows::const_iterator  matchMovingLeft = SearchOneSide (*rightBegin, std::next (leftBegin), leftEnd, columns);
  // advance the right iterator to see if we can match the left begin
  const Rows::const_iterator matchMovingRight = SearchOneSide (*leftBegin, std::next (rightBegin), rightEnd, columns);
  
  // if they both found a match, use the one that moved the least
  const bool foundMovingLeft = (matchMovingLeft != leftEnd);
  const bool foundMovingRight = (matchMovingRight != rightEnd);

  // DEBUG
  const std::string leftMoveSeq = (foundMovingLeft ? (*matchMovingLeft)[1].Repr() : "");
  const std::string rightMoveSeq = (foundMovingRight ? (*matchMovingRight)[1].Repr() : "");
  // \DEBUG
  
  if (foundMovingLeft && foundMovingRight)
  {
    const size_t leftDistance = std::distance (leftBegin, matchMovingLeft);
    const size_t rightDistance = std::distance (rightBegin, matchMovingRight);
    /*** Assume the right side is cannonical.
     * In other words, we expect that every pattern we see on the right
     * side is expected to be found on the left side, excepting for orphans
     * on the left.
     *
     * We still need to be able to detect when there is a legitimate orphan
     * in the cannonical side, so we'll set a threshhold that needs to be
     * breached before we match by advancing the cannonical side.
     */
    static const size_t cannonicalThreshhold = 20;
    if (rightDistance < cannonicalThreshhold)
    {
      // cann. threshhold not breached -- advance the left
      // regardless of distance
      return StepSearchResults (matchMovingLeft, rightBegin, SearchSide::Left);
    }

    // threshhold breached -- advance whichever moved the least
    if (leftDistance <= rightDistance)
      return StepSearchResults (matchMovingLeft, rightBegin, SearchSide::Left);
    else
      return StepSearchResults (leftBegin, matchMovingRight, SearchSide::Right);
  }

  // if we found a match at all, use the one that moved
  if (foundMovingLeft)
  {
    return StepSearchResults (matchMovingLeft, rightBegin, SearchSide::Left);
  }
  if (foundMovingRight)
  {
    return StepSearchResults (leftBegin, matchMovingRight, SearchSide::Right);
  }

  // if we didn't find a match at all, we need to move both sides in order to find a match
  
  /***
   * We'll perform an iterative search.
   * 
   * Step 1) First, we'll set 'anchor points' on each side, one element past the beginning 
   * of the document.
   * 
   * Step 2) Next, We will search the entire right side for a match with the left anchor point.  
   * If we find a match, we're done & we return the results, indicating both sides moved.
   *
   * Step 3) Then we will search the entire left side for a match with the right anchor point,
   * returning if we do find a match.
   *
   * Step 4) If we couldn't find a match in either case, we will then advance both anchor points 
   * and repeat steps 2 & 3 again.  
   *
   * Continue to loop like this until either we find a match or we roll one of the anchor points 
   * past the end of the document -- in which case we indicate no match found.
   *
   */

  // Set our initial anchor points.
  const Rows::const_iterator leftAnchorBeginIt = std::next (leftBegin);
  const Rows::const_iterator rightAnchorBeginIt = std::next (rightBegin);

  for ( Rows::const_iterator leftAnchorIt = leftAnchorBeginIt,
          rightAnchorIt = rightAnchorBeginIt;
        (leftAnchorIt != leftEnd) && (rightAnchorIt != rightEnd);
        ++leftAnchorIt, ++rightAnchorIt)
  {
    const InputRow& leftAnchor = *leftAnchorIt;
    const InputRow& rightAnchor = *rightAnchorIt;

    // DEBUG
    const std::string leftAnchorSeq = leftAnchor[1].Repr();
    const std::string rightAnchorSeq = rightAnchor[1].Repr();
    // \DEBUG
    
    // Search the entire right side for a match with the left anchor
    const Rows::const_iterator iterMatchMovingRightIt = SearchOneSide (leftAnchor, rightAnchorIt, rightEnd, columns);
    if (iterMatchMovingRightIt != rightEnd)
    {
      // DEBUG
      const std::string leftMatchSeq = leftAnchorSeq;
      const std::string rightMatchSeq = (*iterMatchMovingRightIt)[1].Repr();
      // \DEBUG
      
      // We found a match      
      return StepSearchResults (leftAnchorIt, iterMatchMovingRightIt, SearchSide::Both);
    }

    // Search the entire left side for a match with the right anchor
    const Rows::const_iterator iterMatchMovingLeftIt = SearchOneSide (rightAnchor, leftAnchorIt, leftEnd, columns);
    if (iterMatchMovingLeftIt != leftEnd)
    {
      // DEBUG
      const std::string leftMatchSeq = (*iterMatchMovingLeftIt)[1].Repr();
      const std::string rightMatchSeq = rightAnchorSeq;
      // \DEBUG
      
      // We found a match
      return StepSearchResults (iterMatchMovingLeftIt, rightAnchorIt, SearchSide::Both);
    }

    // If we get to here, we advancce both anchors and try again
  }

  // If we get to here, there are no more matches in the entire file.
  // Return total failure, indicated by passing end iterators for 
  // both sides and indicating both sides moved.
  return StepSearchResults (leftEnd, rightEnd, SearchSide::Both);
};

#endif //LMERGE_STEPSEARCH_HPP
