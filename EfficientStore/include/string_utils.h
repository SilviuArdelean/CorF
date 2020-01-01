#pragma once
#include <algorithm>
#include <iomanip>
#include <regex>
#include <sstream>
#include "general.h"

class string_utils {
 public:
  static bool compare_case_sensitive(std::string strFirst,
                                     std::string strSecond) {
#ifdef __linux__
    return (0 == strFirst.compare(strSecond));
#else
    // Convert both strings to upper case by transfrom() before compare.
    std::transform(strFirst.begin(), strFirst.end(), strFirst.begin(), toupper);
    std::transform(strSecond.begin(), strSecond.end(), strSecond.begin(),
                   toupper);

    return (strFirst == strSecond);
#endif
  }

  static bool is_filename(const std::string& filename) {
#ifdef UNICODE
    const std::wregex pattern(_T("^([a-zA-Z0-9s._-]+)$"));
#else
    const std::regex pattern("^([a-zA-Z0-9s._-]+)$");
#endif

    return std::regex_match(filename.cbegin(), filename.cend(), pattern);
  }

  static bool search_substring(const std::string& str,
                               const std::string& sub_str,
                               bool case_sensitive = true) {
    std::size_t pos = 0;

    if (case_sensitive) {
      std::string path2seach(str);
      std::string str4seach(sub_str);

#ifdef _WIN32
      std::transform(path2seach.begin(), path2seach.end(), path2seach.begin(),
                     toupper);
      std::transform(str4seach.begin(), str4seach.end(), str4seach.begin(),
                     toupper);
#endif

      pos = path2seach.find(str4seach);

      return (pos != std::string::npos);
    }

    pos = str.find(sub_str);

    return (pos != std::string::npos);
  }

  static std::string actual_time() {
    time_t now = time(nullptr);

    char buf[64] = {0};
    auto tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return buf;
  }

  // Converts UTC time string to a time_t value.
  // http://arsenmk.blogspot.com/2014/07/converting-string-to-datetime-and-vice.html
  static std::time_t getEpochTime(const std::string& dateTime) {
    // Let's consider we are getting all the input in
    // this format: '2014-07-25T20:17:22Z' (T denotes
    // start of Time part, Z denotes UTC zone).
    // A better approach would be to pass in the format as well.
    static const std::string dateTimeFormat{"%Y-%m-%d %H:%M:%S"};

    // Create a stream which we will use to parse the string,
    // which we provide to constructor of stream to fill the buffer.
    std::istringstream ss{dateTime};

    // Create a tm object to store the parsed date and time.
    std::tm dt;

    // Now we read from buffer using get_time manipulator
    // and formatting the input appropriately.
    ss >> std::get_time(&dt, dateTimeFormat.c_str());

    // Convert the tm structure to time_t value and return.
    return std::mktime(&dt);
  }

  static std::string actual_time2string() {
    time_t now = time(nullptr);

    char buf[64] = {0};
    auto tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return buf;
  }
};
