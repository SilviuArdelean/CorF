#pragma once
#include "general.h"
#include <ctime>
#include <sstream>
#include <iomanip>

// Converts UTC time string to a time_t value.
// http://arsenmk.blogspot.com/2014/07/converting-string-to-datetime-and-vice.html
static std::time_t getEpochTime(const std::string& dateTime)
{
   // Let's consider we are getting all the input in
   // this format: '2014-07-25T20:17:22Z' (T denotes
   // start of Time part, Z denotes UTC zone).
   // A better approach would be to pass in the format as well.
   static const std::string dateTimeFormat{ "%Y-%m-%d %H:%M:%S" };

   // Create a stream which we will use to parse the string,
   // which we provide to constructor of stream to fill the buffer.
   std::istringstream ss{ dateTime };

   // Create a tm object to store the parsed date and time.
   std::tm dt;

   // Now we read from buffer using get_time manipulator
   // and formatting the input appropriately.
   ss >> std::get_time(&dt, dateTimeFormat.c_str());

   // Convert the tm structure to time_t value and return.
   return std::mktime(&dt);
}

static std::string actual_time()
{
   time_t now = time(nullptr);

   char buf[64] = { 0 };
   auto tstruct = *localtime(&now);

   strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
   return buf;
}