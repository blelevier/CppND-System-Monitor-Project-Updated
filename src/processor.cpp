#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// Return the aggregate CPU utilization
// Implementation based on https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() {
    vector<string> processor_data = LinuxParser::CpuUtilization();
    vector<long> cpu_utilization;
    for (string value : processor_data) {
        cpu_utilization.push_back(std::stol(value));
    }
    
    long idle = cpu_utilization[4] + cpu_utilization[5]; //idle + iowait
    //user + nice + system + irq + softirq + steal;
    long nonidle = cpu_utilization[1] + cpu_utilization[2] + cpu_utilization[3] + cpu_utilization[6] + cpu_utilization[7] + cpu_utilization[8];

    // Review how to obtain these data
    long pre_idle = 0;
    long pre_nonidle = 0;

    long total = idle + nonidle;
    long pre_total = pre_idle + pre_nonidle;

    float totald = total - pre_total;
    float idled = idle - pre_idle;
    return (totald - idled) / totald;
}