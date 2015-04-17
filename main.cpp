#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include "std/memory.hpp"

namespace pstd = pico::pstd;

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>

namespace io = boost::iostreams;

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#include <boost/optional.hpp>

namespace b = boost;
typedef io::filtering_istream istream;

#include "util/csv.hpp"

namespace csv = pico::util::csv;

#include "file/header.hpp"

namespace {
  std::unique_ptr<istream> OpenFile (const fs::path &path) {
    std::unique_ptr<istream> ret = pstd::make_unique<istream> ();
    io::file_source fileSrc (path.string ());
    ret->push (fileSrc);
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

  InputRows leftInput;
  InputRows rightInput;

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

  /***  Find the first sync point in each file
   */
  typedef std::pair<InputRows::const_iterator, InputRows::const_iterator> Iters;
  boost::optional<Iters> syncPoint;

  /* we're assuming the connections that created the files were established
   * at the same time.  if that's the case, then the first line in one
   * of the files should be a sync point.
   *
   * So we only need to scan through each file once, comparing the element
   * in that file to the *first* element in the other file.  We'll start
   * by iterating the left file, and if that doesn't find a sync point
   * then iterate through the right file.
   */

  // Iterate left file looking for a sync point
  for (auto it = std::begin (leftInput); !syncPoint && it != std::end (leftInput); ++it) {
//        const auto &leftSeq = (*it)[1]->Repr ();
//        const auto &rightSeq = rightInput.front ()[1]->Repr ();
    if (Equ (*it, rightInput.front (), columns)) {
      syncPoint = std::make_pair (it, rightInput.begin ());
    }
  }

  // If we haven't found the sync point, iterate the right file looking for a sync point
  for (auto it = std::begin (rightInput); !syncPoint && it != std::end (rightInput); ++it) {
//        const auto &rightSeq = (*it)[1]->Repr ();
//        const auto &leftSeq = leftInput.front ()[1]->Repr ();
    if (Equ (*it, leftInput.front (), columns)) {
      syncPoint = std::make_pair (leftInput.begin (), it);
    }
  }

  if (syncPoint) {
    std::clog << "Found sync point between left & right.\nLeft:\n"
    << *(syncPoint->first) << "\n"
    << "Right:\n"
    << *(syncPoint->second) << "\n";
  }

  return 0;

}
