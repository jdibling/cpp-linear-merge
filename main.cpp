#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include "std/memory.hpp"

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>

namespace io = boost::iostreams;

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace b = boost;
typedef io::filtering_istream istream;

#include "util/csv.hpp"

namespace csv = pico::util::csv;

#include "header.hpp"

namespace {
  std::unique_ptr<istream> OpenFile (const fs::path &path) {
    std::unique_ptr<istream> ret = pstd::make_unique<io::filtering_istream> ();
    io::file_source fileSource (path.string ());
    ret->push (fileSource);
    return ret;
  }

}

int main () {
  // Open the source files
  fs::path srcPath = "/home/john/Documents/sample";
  fs::path leftPath = srcPath / "SPY-BD-quote.txt";
  //fs::path leftPath = srcPath / "SPY-Z-quote.txt";
  if (!fs::exists (leftPath)) {
    std::cerr << "Left file '" << leftPath << "' does not exist!" << std::endl;
    return 2;
  }

  fs::path rightPath = srcPath / "SPY-Z-quote.txt";
  //fs::path rightPath = srcPath / "SPY-BD-quote.txt";
  if (!fs::exists (rightPath)) {
    std::cerr << "Right file '" << rightPath << "' does not exist!" << std::endl;
    return 2;
  }

  std::unique_ptr<istream> leftStream = OpenFile (leftPath);
  if (!leftStream) {
    return 3;
  }
  std::cout << "Opened Left file '" << leftPath << "': " << fs::file_size (leftPath) << " bytes." << std::endl;
  std::unique_ptr<istream> rightStream = OpenFile (rightPath);
  if (!rightStream) {
    return 3;
  }
  std::cout << "Opened Right file '" << leftPath << "': " << fs::file_size (rightPath) << " bytes." << std::endl;

  const Columns columns = GetInputFileHeader ();

  InputRows leftInput (columns);
  InputRows rightInput (columns);

  csv::Row csvRow;
  // read the left file
  while ((*leftStream) >> csvRow) {
    leftInput.emplace_back ();
    InputRow &row = leftInput.back ();

    for (size_t ix = 0; ix < csvRow.size (); ++ix) {
      const Column &col = *columns[ix];
      row.push_back (col.CreateCell (csvRow[ix]));
    }
  }
  std::clog << "Read " << leftInput.size () << " lines from left." << "\n";

  // read the right file
  while ((*rightStream) >> csvRow) {
    rightInput.emplace_back ();
    InputRow &row = rightInput.back ();
    for (size_t ix = 0; ix < csvRow.size (); ++ix) {
      const Column &col = *columns[ix];
      row.push_back (col.CreateCell (csvRow[ix]));
    }
  }
  std::clog << "Read " << rightInput.size () << " lines from right." << "\n";

  for (auto row = leftInput.begin (); row != leftInput.end (); ++row) {
    for (auto cell = row->begin (); cell != row->end (); ++cell) {
      std::cout << (*cell)->Repr () << ",";
    }
    std::cout << std::endl;
  }

  /***  Begin step-searching through the files to find the matches
   * and the mismatches
   */

  // define a lambda to determine if we're at the end of both files
  auto AtEnds = [&leftInput, &rightInput] (InputRows::const_iterator lit, InputRows::const_iterator rit) -> bool {
    return (lit == leftInput.end ()) && (rit == rightInput.end ());
  };

  // define a lambda to tell us if these iterators both point to the last elements
  auto AtLasts = [&leftInput, &rightInput] (InputRows::const_iterator lit, InputRows::const_iterator rit) -> bool {
    return (lit == std::prev (leftInput.end ())) && (rit == std::prev (rightInput.end ()));
  };

  // an enum to indicate which iterator was advanced to
  // find a match in a step-search
  enum class SearchSide {
    Left,
    Right,
    Neither, // sides matched from the beginning
    Both,    // no match found -- remaining rows on both sides are orphans
  };

  // a structure to return from the step-search algo
  struct StepSearchResults {
    InputRows::const_iterator mLeftIt;
    InputRows::const_iterator mRightIt;
    SearchSide mWhichAdvanced;

    StepSearchResults (InputRows::const_iterator leftIt, InputRows::const_iterator rightIt, SearchSide side)
      :
      mLeftIt (leftIt),
      mRightIt (rightIt),
      mWhichAdvanced (side) {
    }
  };

  // define a lambda to perform the step-search for a match
  auto StepSearch = [&leftInput,
    &rightInput,
    &columns,
    &AtLasts] (InputRows::const_iterator leftAnchorIt,
               InputRows::const_iterator rightAnchorIt) -> StepSearchResults {
    // initial test for equality
    if (Equ (*leftAnchorIt, *rightAnchorIt, columns)) {
      // match found at beginning
      return StepSearchResults (leftAnchorIt, rightAnchorIt, SearchSide::Neither);
    }

    // the rows we're testing for equality
    InputRows::const_iterator leftIt = leftAnchorIt;
    InputRows::const_iterator rightIt = rightAnchorIt;

    // continue stepping until we reach the last element on both sides
    const InputRows::const_iterator leftLastIt = std::prev (leftInput.end ());
    const InputRows::const_iterator rightLastIt = std::prev (rightInput.end ());
    while (!AtLasts (leftIt, rightIt)) {
      if (Equ (*leftIt, *rightIt, columns)) {
        return StepSearchResults (leftIt, rightIt, SearchSide::Both);
      }
      const std::string &leftAnchorSeq = (*leftAnchorIt)[1]->Repr ();
      const std::string &rightAnchorSeq = (*rightAnchorIt)[1]->Repr ();
      const std::string &leftSeq = (*leftIt)[1]->Repr ();
      const std::string &rightSeq = (*rightIt)[1]->Repr ();

      // iterators pointing to the last element on each side

      const bool leftAnchorIsLast = (leftAnchorIt == leftLastIt);
      const bool leftItIsLast = (leftIt == leftLastIt);
      const bool rightAnchorIsLast = (rightAnchorIt == rightLastIt);
      const bool rightItIsLast = (rightIt == rightLastIt);

      // advance the left and test against the right anchor
      if (!leftAnchorIsLast && !leftItIsLast) {
        leftIt = std::next (leftIt);
        if (Equ (*leftIt, *rightAnchorIt, columns)) {
          // we found a match by advanccing the left side
          return StepSearchResults (leftIt, rightAnchorIt, SearchSide::Left);
        }
      }

      // now advance the right and test against the left anchor
      if (!rightAnchorIsLast && !rightItIsLast) {
        rightIt = std::next (rightIt);
        if (Equ (*leftAnchorIt, *rightIt, columns)) {
          // we found a match by advancing the right side
          return StepSearchResults (leftAnchorIt, rightIt, SearchSide::Right);
        }
      }
    }

    // if we have gotten here, then we have reached the end of both files
    // without finding a match.  We report that both sides were advanced.
    return StepSearchResults (leftIt, rightIt, SearchSide::Both);
  };

  // begin step searching from beginning
  for (InputRows::const_iterator leftRowIt = std::begin (leftInput),
         rightRowIt = std::begin (rightInput);
       !AtEnds (leftRowIt, rightRowIt);
    ) {
    const bool leftEnd = leftRowIt == std::end (leftInput);
    const bool rightEnd = rightRowIt == std::end (rightInput);

    const InputRow &leftRow = *leftRowIt;
    const InputRow &rightRow = *rightRowIt;
    const std::string &leftSeq = leftRow[1]->Repr ();
    const std::string &rightSeq = rightRow[1]->Repr ();

    if (leftSeq == "1399823") {
      bool bk = true;
    }

    StepSearchResults stepSearchRetVal = StepSearch (leftRowIt, rightRowIt);

    // report left orphans, if any
    if (is_one_of (stepSearchRetVal.mWhichAdvanced, SearchSide::Left, SearchSide::Both)) {
      /*** Determine & report the first and last orphans
       */
      InputRows::const_iterator firstOrphan = leftRowIt;
      InputRows::const_iterator lastOrphan = stepSearchRetVal.mLeftIt;
      // if we advanced the left side to find a match, then the found left iter
      // points to a match, not an orphan
      if (stepSearchRetVal.mWhichAdvanced == SearchSide::Left) {
        lastOrphan == std::prev (lastOrphan);
      }

      // report the orphans
      for (auto orphan = firstOrphan; orphan != lastOrphan; ++orphan) {
        const InputRow &orphanRow = *orphan;
        std::cout << orphanRow[1]->Repr () << " <==>" << std::endl;
      }
    }

    // report right orphans, if any
    if (is_one_of (stepSearchRetVal.mWhichAdvanced, SearchSide::Right, SearchSide::Both)) {
      /*** Determine & report the first and last orphans
       */
      InputRows::const_iterator firstOrphan = rightRowIt;
      InputRows::const_iterator lastOrphan = stepSearchRetVal.mRightIt;
      // if we advanced the right side to find a match, then the found right iter
      // points to a match, not an orphan
      if (stepSearchRetVal.mWhichAdvanced == SearchSide::Right) {
        lastOrphan == std::prev (lastOrphan);
      }

      // report the orphans
      for (auto orphan = firstOrphan; orphan != lastOrphan; ++orphan) {
        const InputRow &orphanRow = *orphan;
        std::cout << "\t\t<==>" << orphanRow[1]->Repr () << std::endl;
      }
    }

    // now report the match, if there is one
    //   if (is_one_of (stepSearchRetVal.mWhichAdvanced, SearchSide::Left, SearchSide::Right, SearchSide::Neither))
    {
      const InputRow &leftMatch = *stepSearchRetVal.mLeftIt;
      const InputRow &rightMatch = *stepSearchRetVal.mRightIt;

      std::cout
      << leftMatch[1]->Repr ()
      << "\t<==>\t"
      << rightMatch[1]->Repr ()
      << std::endl;
    }

    // advance and iterate
    leftRowIt = std::next (stepSearchRetVal.mLeftIt);
    rightRowIt = std::next (stepSearchRetVal.mRightIt);
  }


  return 0;

}
