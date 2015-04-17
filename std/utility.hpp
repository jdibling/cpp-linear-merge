//
// Created by john on 4/15/15.
//

#include <string>

#ifndef QC_UTILITY_HPP
#define QC_UTILITY_HPP

// trim from start
static inline std::string ltrim(const std::string &s)
{
    std::string str = s;
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return str;
}

// trim from end
static inline std::string rtrim(const std::string &s)
{
    std::string str = s;
    str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
    return str;
}

// trim from both ends
static inline std::string trim(const std::string &s)
{
    return ltrim(rtrim(s));
}

#endif //QC_UTILITY_HPP
