//
// Created by john on 4/24/15.
//

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/optional.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace io = boost::iostreams;

#include "../std/memory.hpp"

#include "Options.h"

OptionsFactory::OptionsFactory (int argc, char **argv)
  :
  mArgc (argc),
  mArgv (argv) {
}

namespace detail {
  IStreamPtr OpenInputFile (const fs::path &path, bool gzipped) {
    IStreamPtr ret = pstd::make_unique<IStream> ();
    if (gzipped) {
      ret->push (io::gzip_decompressor ());
    }
    io::file_source fileSource (path.string ());
    ret->push (fileSource);
    return ret;
  }

  OStreamPtr OpenOutputFile (boost::optional<fs::path> path, bool gzipped) {
    OStreamPtr ret = pstd::make_unique<OStream> ();
    if (gzipped) {
      ret->push (io::gzip_compressor ());
    }
    if (path) {
      ret->push (io::file_sink (path->string ()));
    }
    else {
      ret->push (std::cout);
    }
    return ret;
  }
}

std::pair<std::string, std::string> parse_negate (const std::string &option) {
  if (option.find ("--no-") == 0) {
    std::string negOption = option.substr (4);
    negOption = "orphans";
    return std::make_pair (negOption, std::string ("false"));
  }
  else {
    return std::make_pair (std::string (), std::string ());
  }
}

std::pair<std::string, std::string> reg_foo (const std::string &s) {
  if (s.find ("--no") == 0) {
    return std::make_pair (s.substr (5), std::string ("false"));
  }
  return std::make_pair (std::string (), std::string ());
}

Options OptionsFactory::Create () const {
  Options ret;
  ret.mTerminate = false;
  ret.mReturnCode = 0;

//  std::string leftPath;
//  std::string rightPath;

  fs::path leftPath;
  fs::path rightPath;
  fs::path outPath;
  bool readGzip = false;
  bool writeGzip = false;

  // pull apart the command line
  po::options_description desc ("Options");
  desc.add_options ()
    ("help,h", "produce help message")
    ("left-file,l", po::value<decltype (leftPath)> (&leftPath), "first file for merging (fq path) [required]")
    ("right-file,r", po::value<decltype (rightPath)> (&rightPath), "second file for merging (fq path) [required]")
    ("out-file,o", po::value<decltype (outPath)> (&outPath),
     "output file for merged files (fq path) [optional, default:stdout]")
    ("read-gzip,z", po::value<bool> (&readGzip)->implicit_value (true)->zero_tokens ()->default_value (false),
     "Input files are gzipped (true|false) [optional, default: false]")
    ("write-gzip,Z", po::value<bool> (&writeGzip)->default_value (false)->implicit_value (true)->zero_tokens (),
     "Output files are gzipped (true|false) [optional, default: false]")
    ("orphans", po::value<decltype (ret.mWriteOrphans)> ()->default_value ("true")->implicit_value ("true"),
     "just an option")
    ("matches", po::value<decltype (ret.mWriteMatches)> ()->default_value ("true")->implicit_value ("true"),
     "just an option");
  po::variables_map vm;
  po::store (po::command_line_parser (mArgc, mArgv)
               .options (desc)
                 //.extra_parser(parse_negate)
               .extra_parser (reg_foo)
               .run (),
             vm);

  ret.mWriteMatches = vm["matches"].as<bool> ();
  ret.mWriteOrphans = vm["orphans"].as<bool> ();
  std::cout << "Write Orphans: " << std::boolalpha << ret.mWriteOrphans << std::endl;
  std::cout << "Write Matches: " << std::boolalpha << ret.mWriteMatches << std::endl;

  // check for help request first
  if (vm.count ("help")) {
    std::cout << "Basic command line help" << std::endl
    << desc << std::endl;
    ret.mTerminate = true;
    ret.mReturnCode = 1;
    return ret;
  }

  po::notify (vm);

  if (readGzip) {
    if (leftPath.extension () != ".gz") {
      fs::path newPath = leftPath.leaf () + ".gz";
      leftPath.remove_leaf () /= newPath;
    }
    if (rightPath.extension () != ".gz") {
      fs::path newPath = rightPath.leaf () + ".gz";
      rightPath.remove_leaf () /= newPath;
    }
  }

  if (writeGzip && vm.count ("out-file") && outPath.extension () != ".gz") {
    fs::path newPath = outPath.leaf () + ".gz";
    outPath.remove_leaf () /= newPath;
  }
  typedef boost::tuple<std::string, std::string, fs::path *, IStreamPtr *> FileCheck;
  typedef std::vector<FileCheck> FileChecks;
  FileChecks fileChecks = {
    boost::make_tuple ("left-file", "Left", &leftPath, &ret.mLeftStream),
    boost::make_tuple ("right-file", "Right", &rightPath, &ret.mRightStream)
  };

  for (auto &check: fileChecks) {
    const std::string &param = boost::get<0> (check);
    const std::string &side = boost::get<1> (check);
    fs::path &path = *boost::get<2> (check);
    IStreamPtr &streamPtr = *boost::get<3> (check);

    // make sure the parameter was specified
    if (!vm.count (param)) {
      std::cerr << "Required parameter missing: " << param << std::endl;
      ret.mTerminate = true;
      ret.mReturnCode = 3;
      return ret;
    }
    //path = vm[param].as<std::remove_reference<decltype (path)>::type> ();

    // get an absolute path & ensure file existance
    path = fs::complete (path);
    if (!fs::exists (path)) {
      std::cerr << side << " file " << path << " not found." << std::endl;
      ret.mTerminate = true;
      ret.mReturnCode = 2;
      return ret;
    }

    // it needs to be a regular file
    if (!fs::is_regular_file (path)) {
      std::cerr << side << " exists, but is not a regular file." << std::endl;
      ret.mTerminate = true;
      ret.mReturnCode = 4;
      return ret;
    }

    // open the file
    streamPtr = detail::OpenInputFile (path, readGzip);
    if (!streamPtr) {
      std::cerr << "Error opening file stream for " << side << " file: " << path << std::endl;
      ret.mTerminate = true;
      ret.mReturnCode = 5;
      return ret;
    }

    // done
    std::clog << "Opened " << side << " file " << path << ", " << fs::file_size (path) << " bytes." << std::endl;
  }

  // parse the output destination
  if (vm.count ("out-file")) {
    ret.mOutputStream = detail::OpenOutputFile (outPath, writeGzip);
    std::clog << "Opened output file " << outPath << std::endl;
  }
  else {
    ret.mOutputStream = detail::OpenOutputFile (boost::none, writeGzip);
    std::clog << "Writing output to stdout" << std::endl;
  }

  // done parsing options, return
  return ret;
}