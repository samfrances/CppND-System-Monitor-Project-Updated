#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "linux_parser_pure.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

LinuxParser::LinuxParser(string root) : kRoot(root) {}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() const {
  std::ifstream filestream(LinuxParserPure::Paths::OperatingSystem(kRoot));
  if (filestream.is_open()) {
    return LinuxParserPure::OperatingSystem(filestream);
  }
  return "";
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() const {
  std::ifstream stream(LinuxParserPure::Paths::Kernel(kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::Kernel(stream);
  }
  return "";
}

// BONUS: Update this to use std::filesystem
// NOTE: My version of gcc does not support std::filesystem
vector<int> LinuxParser::Pids() const {
  vector<int> pids;
  DIR* directory = opendir(LinuxParserPure::Paths::Pids(kRoot).c_str());
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

// DONE: Read and return the system memory utilization
// NOTE: This is a percentage value between 0 and 1
float LinuxParser::MemoryUtilization() const {
  std::ifstream stream(LinuxParserPure::Paths::MemoryUtilization(kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::MemoryUtilization(stream);
  }
  return 0;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() const {
  std::ifstream stream(LinuxParserPure::Paths::UpTime(kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::UpTime(stream);
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) const {
  std::ifstream stream(
      LinuxParserPure::Paths::ActiveJiffiesForProcess(pid, kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::ActiveJiffiesForProcess(stream);
  }
  return 0;
}

// DONE: Read and return CPU utilization
LinuxParserPure::CpuUtilizationSnapshot LinuxParser::CpuUtilization() const {
  std::ifstream stream(LinuxParserPure::Paths::CpuUtilization(kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::CpuUtilization(stream);
  }
  return {};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() const {
  std::ifstream stream(LinuxParserPure::Paths::TotalProcesses(kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::TotalProcesses(stream);
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() const {
  std::ifstream stream(LinuxParserPure::Paths::RunningProcesses(kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::RunningProcesses(stream);
  }
  return 0;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) const {
  std::ifstream stream(LinuxParserPure::Paths::Command(pid, kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::Command(stream);
  }
  return "";
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) const {
  std::ifstream stream(LinuxParserPure::Paths::Ram(pid, kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::Ram(stream);
  }
  return "";
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) const {
  std::ifstream stream(LinuxParserPure::Paths::Uid(pid, kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::Uid(stream);
  }
  return "";
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) const {
  string uid = Uid(pid);
  if (!uid.empty()) {
    std::ifstream stream(LinuxParserPure::Paths::User(kRoot));
    if (stream.is_open()) {
      return LinuxParserPure::User(stream, uid);
    }
  }
  return "";
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) const {
  std::ifstream stream(LinuxParserPure::Paths::ProcessUpTime(pid, kRoot));
  if (stream.is_open()) {
    return LinuxParserPure::ProcessUpTime(stream);
  }
  return 0;
}
