//
// Created by john on 4/17/15.
//

#ifndef LMERGE_CONVERT_HPP
#define LMERGE_CONVERT_HPP

#include <string>
#include <sstream>

template<typename Val>
inline Val Convert (const std::string &str) {
  std::stringstream ss;
  ss << str;
  Val ret;
  ss >> ret;
  return ret;
}

#endif //LMERGE_CONVERT_HPP
