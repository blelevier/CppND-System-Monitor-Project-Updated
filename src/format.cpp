#include <string>
#include <iomanip>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

const int HOUR_TO_SECONDS = 3600;
const int MINUTE_TO_SECONDS = 60;

string Format::ElapsedTime(long total_time_sec) { 
    int hours = total_time_sec / HOUR_TO_SECONDS;
    int remaining_time = total_time_sec % HOUR_TO_SECONDS;
    int minutes = remaining_time / MINUTE_TO_SECONDS;
    int seconds = remaining_time % MINUTE_TO_SECONDS;

    std::stringstream time_formatted;
    time_formatted << std::setprecision(2)<< std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
    
    return time_formatted.str(); 
}