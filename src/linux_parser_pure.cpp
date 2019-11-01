#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser_pure.h"

using std::string;

// "Private" namespace
namespace {
int ParseProcStat(std::istream& filestream, string desired_key) {
  string line;
  string key;
  int value;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == desired_key) {
      return value;
    }
  }
  return 0;
}

/**
 * Parse a /proc/<pid>/status file, searching for a specified key, and
 * return the first of the space separated values for that key, if it
 * is a numerical string. Otherwise return "".
 */
string ParseProcStatusDigit(std::istream& filestream, string desired_key) {
  string line;
  string key;
  string value;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == desired_key + ":") {
      if (std::all_of(value.begin(), value.end(), isdigit)) {
        return value;
      }
      break;
    }
  }
  return "";
}
}

float LinuxParserPure::MemoryUtilization(std::istream& filestream) {
  string line;
  string key;
  bool totalFound = false;
  float total;
  bool freeFound = false;
  float free;
  float value;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "MemTotal:") {
      total = value;
      totalFound = true;
    } else if (key == "MemFree:") {
      free = value;
      freeFound = true;
    }

    if (totalFound && freeFound) {
      return (total - free) / total;
    }
  }
  return 0.0;
}

string LinuxParserPure::Paths::MemoryUtilization(std::string root) {
  return root + kProcDirectory + kMeminfoFilename;
}

long LinuxParserPure::UpTime(std::istream& filestream) {
  long uptime;
  string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> uptime;
  return uptime;
}

string LinuxParserPure::Paths::UpTime(std::string root) {
  return root + kProcDirectory + kUptimeFilename;
}

int LinuxParserPure::TotalProcesses(std::istream& filestream) {
  return ParseProcStat(filestream, "processes");
}

string LinuxParserPure::Paths::ProcStat(std::string root) {
  return root + kProcDirectory + kStatFilename;
}

string LinuxParserPure::Paths::TotalProcesses(std::string root) {
  return ProcStat(root);
}

int LinuxParserPure::RunningProcesses(std::istream& filestream) {
  return ParseProcStat(filestream, "procs_running");
}

string LinuxParserPure::Paths::RunningProcesses(std::string root) {
  return ProcStat(root);
}

string LinuxParserPure::OperatingSystem(std::istream& filestream) {
  string key;
  string value;
  string line;
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
  return "";
}

string LinuxParserPure::Paths::OperatingSystem(std::string root) {
  return root + kOSPath;
}

string LinuxParserPure::Kernel(std::istream& filestream) {
  string os, version, kernel;
  string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> os >> version >> kernel;
  return kernel;
}

string LinuxParserPure::Paths::Kernel(std::string root) {
  return root + kProcDirectory + kVersionFilename;
}

string LinuxParserPure::Paths::Pids(std::string root) {
  return root + kProcDirectory;
}

LinuxParserPure::CpuUtilizationSnapshot LinuxParserPure::CpuUtilization(
    std::istream& filestream) {
  string line;
  string word;
  std::vector<long> result;
  if (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> word;  // get rid of "cpu" word
    while (linestream >> word) {
      result.push_back(stol(word));
    }
  }
  return CpuUtilizationSnapshot(result.at(0), result.at(1), result.at(2),
                                result.at(3), result.at(4), result.at(5),
                                result.at(6), result.at(7));
}

string LinuxParserPure::Paths::CpuUtilization(std::string root) {
  return ProcStat(root);
}

LinuxParserPure::CpuUtilizationSnapshot::CpuUtilizationSnapshot(
    long user, long nice, long system, long idle, long iowait, long irq,
    long softirq, long steal)
    : user(user),
      nice(nice),
      system(system),
      idle(idle),
      iowait(iowait),
      irq(irq),
      softirq(softirq),
      steal(steal) {}

LinuxParserPure::CpuUtilizationSnapshot::CpuUtilizationSnapshot()  // TODO
                                                                   // remove
    : user(0),
      nice(0),
      system(0),
      idle(0),
      iowait(0),
      irq(0),
      softirq(0),
      steal(0) {}

long LinuxParserPure::CpuUtilizationSnapshot::Jiffies() {
  return user + nice + system + idle + iowait + irq + softirq + steal;
}

long LinuxParserPure::CpuUtilizationSnapshot::IdleJiffies() {
  return idle + iowait;
}

long LinuxParserPure::CpuUtilizationSnapshot::ActiveJiffies() {
  return Jiffies() - IdleJiffies();
}

// Helper method to produce the /proc/<pid>/ directory string
string LinuxParserPure::Paths::ProcessDir(int pid, string root) {
  return root + kProcDirectory + std::to_string(pid) + "/";
}

string LinuxParserPure::Command(std::istream& filestream) {
  string line;
  if (std::getline(filestream, line)) {
    return line;
  }
  return "";
}

string LinuxParserPure::Paths::Command(int pid, std::string root) {
  return ProcessDir(pid, root) + kCmdlineFilename;
}

string LinuxParserPure::Ram(std::istream& filestream) {
  string value = ParseProcStatusDigit(filestream, "VmSize");
  if (value == "") {
    return value;
  }
  return std::to_string(stoi(value) / 1024.0);
}

string LinuxParserPure::Paths::Ram(int pid, std::string root) {
  return ProcessDir(pid, root) + kStatusFilename;
}

string LinuxParserPure::Uid(std::istream& filestream) {
  return ParseProcStatusDigit(filestream, "Uid");
}

string LinuxParserPure::Paths::Uid(int pid, std::string root) {
  return ProcessDir(pid, root) + kStatusFilename;
}

string LinuxParserPure::User(std::istream& filestream, string desired_uid) {
  string line;
  string username;
  string pwd;
  string uid;
  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> username >> pwd >> uid;
    if (uid == desired_uid) {
      return username;
    }
  }
  return "";
}

string LinuxParserPure::Paths::User(std::string root) {
  return root + kPasswordPath;
}

long int LinuxParserPure::ProcessUpTime(std::istream& filestream) {
  string line;
  if (std::getline(filestream, line)) {
    string starttime;
    std::istringstream contents(line);
    for (int i = 1; i <= 22; i++) {
      contents >> starttime;
    }
    return stol(starttime) / sysconf(_SC_CLK_TCK);
  }
}

string LinuxParserPure::Paths::ProcessUpTime(int pid, std::string root) {
  return ProcessDir(pid, root) + kStatFilename;
}

long int LinuxParserPure::ActiveJiffiesForProcess(std::istream& filestream) {
  string line;
  if (std::getline(filestream, line)) {
    long utime = 0;
    long stime = 0;
    string garbage;
    std::istringstream contents(line);
    for (int i = 1; i <= 15; i++) {
      if (i == 14) {
        contents >> utime;
      } else if (i == 15) {
        contents >> stime;
      } else {
        contents >> garbage;
      }
    }
    return utime + stime;
  }
}
