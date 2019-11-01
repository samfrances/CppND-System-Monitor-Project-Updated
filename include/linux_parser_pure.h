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

  // CPU
  std::vector<std::string> CpuUtilization(); // TODO
  long Jiffies(std::istream&); // TODO
  long ActiveJiffies(std::istream&); // TODO
  long ActiveJiffiesForProcess(std::istream&); // TODO
  long IdleJiffies(std::istream&); // TODO

  // Processes
  std::string Command(std::istream&);
  std::string Ram(std::istream&);
  std::string Uid(std::istream&);
  std::string User(std::istream&, std::string uid);
  long int ProcessUpTime(std::istream&);

  // Paths of system files
  namespace Paths {

    // System
    std::string MemoryUtilization(std::string root = "/");
    std::string UpTime(std::string root = "/");
    std::string Pids(std::string root = "/");
    std::string TotalProcesses(std::string root = "/");
    std::string RunningProcesses(std::string root = "/");
    std::string OperatingSystem(std::string root = "/");
    std::string Kernel(std::string root = "/");

    // Processes
    std::string Command(int pid, std::string root = "/"); // TODO
    std::string Ram(int pid, std::string root = "/"); // TODO
    std::string Uid(int pid, std::string root = "/"); // TODO
    std::string User(int pid, std::string root = "/"); // TODO
    std::string UpTime(int pid, std::string root = "/"); // TODO

    // Paths
    const std::string kProcDirectory{"proc/"};
    const std::string kCmdlineFilename{"/cmdline"};
    const std::string kCpuinfoFilename{"/cpuinfo"};
    const std::string kStatusFilename{"/status"};
    const std::string kStatFilename{"stat"};
    const std::string kUptimeFilename{"uptime"};
    const std::string kMeminfoFilename{"meminfo"};
    const std::string kVersionFilename{"version"};
    const std::string kOSPath{"etc/os-release"};
    const std::string kPasswordPath{"/etc/passwd"};

  } // namespace Paths

};  // namespace LinuxParserPure

#endif