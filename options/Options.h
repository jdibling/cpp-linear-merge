//
// Created by john on 4/24/15.
//

#ifndef LMERGE_OPTIONS_H
#define LMERGE_OPTIONS_H

#include <boost/filesystem.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp>

typedef boost::iostreams::filtering_istream IStream;
typedef boost::iostreams::filtering_ostream OStream;
typedef std::unique_ptr<IStream> IStreamPtr;
typedef std::unique_ptr<OStream> OStreamPtr;

class Options {
public:
  bool mTerminate;
  int mReturnCode;
  IStreamPtr mLeftStream;
  IStreamPtr mRightStream;
  OStreamPtr mOutputStream;
  OStreamPtr mLogStream;
  bool mWriteOrphans;
  bool mWriteMatches;
  bool mTrimOrphans; // if true, don't report orphans occuring at the beginning or end of the input
};

class OptionsFactory {
  int mArgc;
  char **mArgv;
public:
  OptionsFactory (int argc, char **argv);

  Options Create () const;
};

#endif //LMERGE_OPTIONS_H
