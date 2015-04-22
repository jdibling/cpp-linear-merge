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
#include "row/Row.hpp"
#include "StepSearch.hpp"
#include "std/utility.hpp"

namespace csv = pico::util::csv;

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
//  fs::path srcPath = "/home/john/Documents/sample";
//  fs::path leftPath = srcPath / "SPY-BD-quote.txt";
//  fs::path rightPath = srcPath / "SPY-Z-quote.txt";

  fs::path srcPath = "/home/john/bbo/20150421";
  fs::path leftPath = srcPath / "/TotalView/ID/quote/SPY-ID-quote.txt";
  fs::path rightPath = srcPath / "/Cqs/T/quote/SPY-T-quote.txt";

  if (!fs::exists (leftPath)) {
    std::cerr << "Left file '" << leftPath << "' does not exist!" << std::endl;
    return 2;
  }

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

  // Get ready to read the inputs
  Columns inputColumns {
    Column ("RecvTime", CellType::UInt),
    Column ("SeqNum", CellType::UInt),
    Column ("Ticker", CellType::Text),
    Column ("BidSide", CellType::Text),
    Column ("BidPrice", CellType::Text, Importance::Key),
    Column ("BidSizeRaw", CellType::UInt),
    Column ("BidSize", CellType::UInt, Importance::Key),
    Column ("BidTime", CellType::Text),
    Column ("BidVenue", CellType::Text),
    Column ("AskSide", CellType::Text),
    Column ("AskPrice", CellType::Text, Importance::Key),
    Column ("AskSizeRaw", CellType::UInt),
    Column ("AskSize", CellType::UInt, Importance::Key),
    Column ("AskTime", CellType::Text),
    Column ("AskVenue", CellType::Text)
  };

  csv::Row csvRow;
  // read the left file & create an input row
  Rows leftInput;
  while ((*leftStream) >> csvRow) {
    Row inputRow;

    for (size_t idx = 0; idx < csvRow.size (); ++idx) {
      const std::string &cellData = csvRow[idx];
      const Column &col = inputColumns[idx];
      inputRow.push_back (col.CreateInputCell (cellData));
    }

    leftInput.push_back (std::move (inputRow));
  }
  std::clog << "Read " << leftInput.size () << " lines from left." << "\n";
  for (size_t idx = 0; idx < leftInput.size (); ++idx) {
    const Row &row = leftInput[idx];
    std::clog << "[" << idx + 1 << "]:\t" << row << std::endl;
  }
  
  // read the right file & create an input row
  Rows rightInput;
  while ((*rightStream) >> csvRow) {
    Row inputRow;

    for (size_t idx = 0; idx < csvRow.size (); ++idx) {
      const std::string &cellData = csvRow[idx];
      const Column &col = inputColumns[idx];
      inputRow.push_back (col.CreateInputCell (cellData));
    }

    rightInput.push_back (std::move (inputRow));
  }
  std::clog << "Read " << rightInput.size () << " lines from right." << "\n";
  for (size_t idx = 0; idx < rightInput.size (); ++idx) {
    const Row &row = rightInput[idx];
    std::clog << "[" << idx + 1 << "]:\t" << row << std::endl;
  }

  // begin step searching from beginning
  const Rows::const_iterator leftEnd = std::end (leftInput);
  const Rows::const_iterator rightEnd = std::end (rightInput);

  for (Rows::const_iterator leftRowIt = std::begin (leftInput),
         rightRowIt = std::begin (rightInput);
       !(leftRowIt == leftEnd && rightRowIt == rightEnd);
    ) {
    const bool atLeftEnd = (leftRowIt == leftEnd);
    const bool atRightEnd = (rightRowIt == rightEnd);
    const Row &leftRow = *leftRowIt;
    const Row &rightRow = *rightRowIt;
    const std::string &leftSeq = leftRow[1].Repr ();
    const std::string &rightSeq = rightRow[1].Repr ();

    StepSearchResults stepSearchRetVal = StepSearch (leftRowIt, leftEnd, rightRowIt, rightEnd, inputColumns);

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
      for (auto orphan = firstOrphan; orphan != lastOrphan; ++orphan) {
        const Row &orphanRow = *orphan;
        std::clog << orphanRow[1].Repr () << " <==>" << std::endl;
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
      for (auto orphan = firstOrphan; orphan != lastOrphan; ++orphan) {
        const Row &orphanRow = *orphan;
        std::clog << "\t\t<==>\t" << orphanRow[1].Repr () << std::endl;
      }
    }

    // now report the match, if there is one
    //   if (is_one_of (stepSearchRetVal.mWhichAdvanced, SearchSide::Left, SearchSide::Right, SearchSide::Neither))
    {
      const Row &leftMatch = *stepSearchRetVal.mLeftIt;
      const Row &rightMatch = *stepSearchRetVal.mRightIt;

//      CellFactory cf (CellType::Latency);
//      CellPtr latencyCell = cf.CreateMergeCell (leftMatch, rightMatch);

      std::clog
      << leftMatch[1].Repr ()
      << "\t<==>\t"
      << rightMatch[1].Repr ()
//         << "\t" << latencyCell->Repr ()
      << std::endl;
    }

    // advance and iterate
    leftRowIt = std::next (stepSearchRetVal.mLeftIt);
    rightRowIt = std::next (stepSearchRetVal.mRightIt);
  }


  return 0;

}
