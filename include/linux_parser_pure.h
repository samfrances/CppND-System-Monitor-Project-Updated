#ifndef SYSTEM_PARSER_PURE_H
#define SYSTEM_PARSER_PURE_H

#include <istream>
#include <regex>
#include <string>

/**
 * Pure parsing methods, without any file IO
 */
namespace LinuxParserPure {

// System
float MemoryUtilization(std::istream&);
long UpTime(std::istream&);
int TotalProcesses(std::istream&);
int RunningProcesses(std::istream&);
std::string OperatingSystem(std::istream&);
std::string Kernel(std::istream&);

// CPU
class CpuUsage {
 public:
  CpuUsage(long user, long nice, long system, long idle, long iowait, long irq,
           long softirq, long steal)
      : user(user),
        nice(nice),
        system(system),
        idle(idle),
        iowait(iowait),
        irq(irq),
        softirq(softirq),
        steal(steal) {}
  const long user = 0;
  const long nice = 0;
  const long system = 0;
  const long idle = 0;
  const long iowait = 0;
  const long irq = 0;
  const long softirq = 0;
  const long steal = 0;
};

CpuUsage CpuUtilization(std::istream&);
long Jiffies(std::vector<std::string>);
long ActiveJiffies(std::vector<std::string>);  // TODO
long ActiveJiffiesForProcess(std::istream&);   // TODO
long IdleJiffies(std::vector<std::string>);    // TODO

// Processes
std::string Command(std::istream&);
std::string Ram(std::istream&);
std::string Uid(std::istream&);
std::string User(std::istream&, std::string uid);
long int ProcessUpTime(std::istream&);

// Paths of system files
namespace Paths {

// Helpers
std::string ProcessDir(int pid, std::string root);
std::string ProcStat(std::string root = "/");

// System
std::string MemoryUtilization(std::string root = "/");
std::string UpTime(std::string root = "/");
std::string Pids(std::string root = "/");
std::string TotalProcesses(std::string root = "/");
std::string RunningProcesses(std::string root = "/");
std::string OperatingSystem(std::string root = "/");
std::string Kernel(std::string root = "/");

// CPU
std::string CpuUtilization(std::string root = "/");

// Processes
std::string Command(int pid, std::string root = "/");
std::string Ram(int pid, std::string root = "/");
std::string Uid(int pid, std::string root = "/");
std::string User(std::string root = "/");
std::string ProcessUpTime(int pid, std::string root = "/");

// Paths
const std::string kProcDirectory{"proc/"};
const std::string kCmdlineFilename{"cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"status"};
const std::string kStatFilename{"stat"};
const std::string kUptimeFilename{"uptime"};
const std::string kMeminfoFilename{"meminfo"};
const std::string kVersionFilename{"version"};
const std::string kOSPath{"etc/os-release"};
const std::string kPasswordPath{"etc/passwd"};

}  // namespace Paths

};  // namespace LinuxParserPure

#endif