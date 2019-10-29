#include <string>

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
    }
    else if (key == "MemFree:") {
      free = value;
      freeFound = true;
    }

    if (totalFound && freeFound) {
      return (total - free) / total;
    }

  }
  return 0.0;
}

long LinuxParserPure::UpTime(std::istream& filestream) {
  long uptime;
  string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> uptime;
  return uptime;
}

int LinuxParserPure::TotalProcesses(std::istream& filestream) {
  return ParseProcStat(filestream, "processes");
}

int LinuxParserPure::RunningProcesses(std::istream& filestream) {
  return ParseProcStat(filestream, "procs_running");
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

string LinuxParserPure::Kernel(std::istream& filestream) {
  string os, version, kernel;
  string line;
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> os >> version >> kernel;
  return kernel;
}

string LinuxParserPure::Command(std::istream& filestream) {
  string line;
  if (std::getline(filestream, line)) {
    return line;
  }
  return "";
}

string LinuxParserPure::Ram(std::istream& filestream) {
  string line;
  string key;
  string value;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "VmSize:") {
      if (std::all_of(value.begin(), value.end(), isdigit)) {
        return std::to_string(stoi(value) / 1024.0);
      }
      break;
    }
  }
  return "";
}
