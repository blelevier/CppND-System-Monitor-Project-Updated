#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, number, line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> number;
  }
  return kernel + " " + number;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float mem_total = 1;
  float mem_free = 1;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        mem_total = stof(value);
      } else if (key == "MemFree:"){
        mem_free = stof(value); 
      }
    }
  }
  return (mem_total - mem_free) / (mem_total); 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    long up_time;
    filestream >> up_time;
    return up_time; 
  }
  return 0;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, key, value;
  vector<string> processor_data;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      processor_data.push_back(value);
    }
  }
  return processor_data;
}

// Read and return CPU utilization
// Calculation based on https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float LinuxParser::CpuUtilization(int pid) { 

  const int index_utime = 14;
  const int index_stime = 15;
  const int index_cutime = 16;
  const int index_cstime = 17;
  const int index_starttime = 22;

  long uptime = UpTime();
  long utime = 0;
  long stime = 0;
  long cutime = 0;
  long cstime = 0;
  long starttime = 0; 

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line, value;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream linestream(line);
    int count = 0;
    while (linestream >> value) {
      count += 1;
      switch (count) {
        case index_utime:
          utime = stol(value);
          break;
        case index_stime:
          stime = stol(value);
          break;
        case index_cutime:
          cutime = stol(value);
          break;
        case index_cstime:
          cstime = stol(value);
          break;
        case index_starttime:
          starttime = stol(value);
          break;
      }
    }
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
    return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  }
  return 0.0;  
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  int proc_total = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        proc_total = std::stoi(value);
      } 
    }
  }
  return proc_total; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, key, value;
  int proc_running = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        proc_running = std::stoi(value);
      } 
    }
  }
  return proc_running;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    string command;
    filestream >> command;
    return command; 
  }
  return string();
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line, key, value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        long total_ram =stof(value) / 1000;
        return  std::to_string(total_ram);
      } 
    }
  }
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line, key, value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      } 
    }
  }
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  string line, user, x, value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> value;
      if (value == uid) {
        return user;
      } 
    }
  }
  return "N/A";  
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  const int index_uptime = 22;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line, user, x, value;
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    int count = 0;
    std::istringstream linestream(line);
    while (linestream >> value){
      count += 1;
      if (count == index_uptime){
        return UpTime() - (stol(value) / sysconf(_SC_CLK_TCK));
      }
    } 
  }
  return 0;
}