#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_number; }

// Return this process's CPU utilization
float Process::CpuUtilization() const{ return LinuxParser::CpuUtilization(pid_number); }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_number); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_number); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_number); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_number); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return (CpuUtilization() > a.CpuUtilization()); 
}