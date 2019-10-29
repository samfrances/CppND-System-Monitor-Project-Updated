/**
 * Pure parsing methods, without any file IO
 */

#ifndef SYSTEM_PARSER_PURE_H
#define SYSTEM_PARSER_PURE_H

#include <istream>
#include <regex>
#include <string>

namespace LinuxParserPure {

  // System
  float MemoryUtilization(std::istream&);
  long UpTime(std::istream&);
  int TotalProcesses(std::istream&);
  int RunningProcesses(std::istream&);
  std::string OperatingSystem(std::istream&);
  std::string Kernel(std::istream&);

  // Processes
  std::string Command(std::istream&);
  std::string Ram(std::istream&); // TODO
  std::string Uid(std::istream&); // TODO
  std::string User(std::istream&, std::string uid); // TODO
  long int UpTime(std::istream&); // TODO

  // Paths of system files
  namespace Paths {

    // System
    std::string MemoryUtilization(std::string root = "/"); // TODO
    std::string UpTime(std::string root = "/"); // TODO
    std::string TotalProcesses(std::string root = "/"); // TODO
    std::string RunningProcesses(std::string root = "/"); // TODO
    std::string OperatingSystem(std::string root = "/"); // TODO
    std::string Kernel(std::string root = "/"); // TODO

    // User
    std::string Command(int pid, std::string root = "/"); // TODO
    std::string Ram(int pid, std::string root = "/"); // TODO
    std::string Uid(int pid, std::string root = "/"); // TODO
    std::string User(int pid, std::string root = "/"); // TODO
    std::string UpTime(int pid, std::string root = "/"); // TODO
  } // namespace Paths

};  // namespace LinuxParserPure

#endif