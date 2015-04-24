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
#include "column/Column.hpp"
#include "StepSearch.hpp"
#include "row/OutputRowFactory.hpp"

#include "options/Options.h"

namespace csv = pico::util::csv;

namespace {
  std::unique_ptr<istream> OpenFile (const fs::path &path) {
    std::unique_ptr<istream> ret = pstd::make_unique<io::filtering_istream> ();
    io::file_source fileSource (path.string ());
    ret->push (fileSource);
    return ret;
  }

}

int main (int argc, char **argv) {
  OptionsFactory optionsFactory (argc, argv);
  const Options options = optionsFactory.Create ();
  if (options.mTerminate) {
    return 0;
  }

  OStream &LStr = *options.mLogStream;

  // Get ready to read the inputs
  Columns inputColumns {
    Column ("RecvTime", UInt ()),
    Column ("SeqNum", UInt ()),
    Column ("Ticker", Text ()),
    Column ("BidSide", Text ()),
    Column ("BidPrice", Text (), Importance::Key),
    Column ("BidSizeRaw", UInt ()),
    Column ("BidSize", UInt (), Importance::Key),
    Column ("BidTime", Text ()),
    Column ("BidVenue", Text ()),
    Column ("AskSide", Text ()),
    Column ("AskPrice", Text (), Importance::Key),
    Column ("AskSizeRaw", UInt ()),
    Column ("AskSize", UInt (), Importance::Key),
    Column ("AskTime", Text ()),
    Column ("AskVenue", Text ())
  };

  Columns outputColumns {
    Column ("LatDiff", Diff (), Importance::Output)
  };

//  Columns testColumns {
//    Column ("LatDiff", Diff (Right ("RecvTime"), Left("RecvTime")));
//  };

  csv::Row csvRow;
  // read the left file & create an input row
  Rows leftInput;
  LStr << "Reading from left... ";
  while ((*options.mLeftStream) >> csvRow) {
    if (csvRow.size () != inputColumns.size ()) {
      LStr << "Incomplete or malformed input from left: '" << csvRow.Raw () << "' -- skipping." << std::endl;
      continue;
    }
    InputRow inputRow;

    for (size_t idx = 0; idx < csvRow.size (); ++idx) {
      const std::string &cellData = csvRow[idx];
      const Column &col = inputColumns[idx];
      inputRow.push_back (col.CreateInputCell (cellData));
    }

    leftInput.push_back (std::move (inputRow));
  }
  LStr << "Done." << std::endl;

  // read the right file & create an input row
  Rows rightInput;
  LStr << "Reading from right... ";
  while ((*options.mRightStream) >> csvRow) {
    if (csvRow.size () != inputColumns.size ()) {
      LStr << "Incomplete or malformed input from right: '" << csvRow.Raw () << "' -- skipping." << std::endl;
      continue;
    }
    InputRow inputRow;

    for (size_t idx = 0; idx < csvRow.size (); ++idx) {
      const std::string &cellData = csvRow[idx];
      const Column &col = inputColumns[idx];
      inputRow.push_back (col.CreateInputCell (cellData));
    }

    rightInput.push_back (std::move (inputRow));
  }
  LStr << "Done." << std::endl;
  // begin step searching from beginning
  const Rows::const_iterator leftEnd = std::end (leftInput);
  const Rows::const_iterator rightEnd = std::end (rightInput);
  OutputRowFactory outRowFactory (inputColumns, outputColumns);

  bool firstSearch = true;

  size_t leftOrphans = 0;
  size_t rightOrphans = 0;

  for (Rows::const_iterator leftRowIt = std::begin (leftInput),
         rightRowIt = std::begin (rightInput);
       !(leftRowIt == leftEnd && rightRowIt == rightEnd);
       firstSearch = false
    ) {
    const bool atLeftEnd = (leftRowIt == leftEnd);
    const bool atRightEnd = (rightRowIt == rightEnd);
    const InputRow &leftRow = *leftRowIt;
    const InputRow &rightRow = *rightRowIt;

    StepSearchResults stepSearchRetVal = StepSearch (leftRowIt, leftEnd, rightRowIt, rightEnd, inputColumns,
                                                     firstSearch);

    // report left orphans, if any
    if (is_one_of (stepSearchRetVal.mWhichAdvanced, SearchSide::Left, SearchSide::Both)) {
      /*** Determine & report the first and last orphans
       */
      Rows::const_iterator firstOrphan = leftRowIt;
      Rows::const_iterator lastOrphan = stepSearchRetVal.mLeftIt;
      // if we advanced the left side to find a match, then the found left iter
      // points to a match, not an orphan
      if (stepSearchRetVal.mWhichAdvanced == SearchSide::Left) {
        lastOrphan == std::prev (lastOrphan);
      }

      // report the orphans
      leftOrphans += std::distance (firstOrphan, lastOrphan);
      if (options.mWriteOrphans) {
        for (auto orphan = firstOrphan; orphan != lastOrphan; ++orphan) {
          const InputRow &orphanRow = *orphan;
          OutputRowPtr outRowPtr = outRowFactory.CreateOutputRow (orphanRow, boost::none);
          OutputRow &outRow = *outRowPtr;
          (*options.mOutputStream) << outRow << std::endl;
        }
      }
    }

    // report right orphans, if any
    if (is_one_of (stepSearchRetVal.mWhichAdvanced, SearchSide::Right, SearchSide::Both)) {
      /*** Determine & report the first and last orphans
       */
      Rows::const_iterator firstOrphan = rightRowIt;
      Rows::const_iterator lastOrphan = stepSearchRetVal.mRightIt;
      // if we advanced the right side to find a match, then the found right iter
      // points to a match, not an orphan
      if (stepSearchRetVal.mWhichAdvanced == SearchSide::Right) {
        lastOrphan == std::prev (lastOrphan);
      }

      // report the orphans
      rightOrphans += std::distance (firstOrphan, lastOrphan);
      if (options.mWriteOrphans) {
        for (auto orphan = firstOrphan; orphan != lastOrphan; ++orphan) {
          const InputRow &orphanRow = *orphan;
          OutputRowPtr outRowPtr = outRowFactory.CreateOutputRow (boost::none, orphanRow);
          OutputRow &outRow = *outRowPtr;

          (*options.mOutputStream) << outRow << std::endl;
        }
      }
    }

    // now report the match, if there is one
    if (options.mWriteMatches) {
      if (!atLeftEnd && !atRightEnd) {
        const InputRow &leftMatch = *stepSearchRetVal.mLeftIt;
        const InputRow &rightMatch = *stepSearchRetVal.mRightIt;

        OutputRowPtr outRowPtr = outRowFactory.CreateOutputRow (leftMatch, rightMatch);
        OutputRow &outRow = *outRowPtr;

        (*options.mOutputStream) << outRow << std::endl;
      }
    }

    // advance and iterate
    if (!atLeftEnd) {
      leftRowIt = std::next (stepSearchRetVal.mLeftIt);
    }
    if (!atRightEnd) {
      rightRowIt = std::next (stepSearchRetVal.mRightIt);
    }
  }

  LStr << "Left orphans: " << leftOrphans << "\n"
  << "Right orphans: " << rightOrphans << "\n"
  << "Total orphans: " << leftOrphans + rightOrphans
  << std::endl;

  return 0;

}
