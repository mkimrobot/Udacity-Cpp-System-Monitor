#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "format.h"

#define HOUR_SEC 3600
#define MIN_SEC 60

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
string Format::ElapsedTime(long seconds) { 
    int hour, min, sec;
    string time;
    std::ostringstream h, m, s;

    std::cout << seconds << " ";

    hour = seconds / HOUR_SEC;
    min = (seconds - (hour * HOUR_SEC)) / MIN_SEC;
    sec = seconds - (hour * HOUR_SEC) - (min * MIN_SEC);

    h << std::internal << std::setfill('0') << std::setw(2) << hour;
    m << std::internal << std::setfill('0') << std::setw(2) << min;
    s << std::internal << std::setfill('0') << std::setw(2) << sec;

    time = h.str() + ":" + m.str() + ":" + s.str();
    return time;
}