#include <string>

#include "linux_parser_pure.h"

using std::string;

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

int LinuxParserPure::TotalProcesses(std::istream& filestream) {
  string line;
  string key;
  int value;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "processes") {
      return value;
    }
  }
  return 0;
}