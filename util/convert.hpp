//
// Created by john on 4/17/15.
//

#ifndef LMERGE_CONVERT_HPP
#define LMERGE_CONVERT_HPP

#include <string>
#include <sstream>

template<typename Out, typename In=std::string>
inline Out Convert (const In &str) {
  std::stringstream ss;
  ss << str;
  Out ret;
  ss >> ret;
  return ret;
}

#endif //LMERGE_CONVERT_HPP
