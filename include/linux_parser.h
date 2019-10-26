#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

class ILinuxParser {
public:
  // System
  virtual float MemoryUtilization() = 0;
  virtual long UpTime() = 0;
  virtual std::vector<int> Pids() = 0;
  virtual int TotalProcesses() = 0;
  virtual int RunningProcesses() = 0;
  virtual std::string OperatingSystem() = 0;
  virtual std::string Kernel() = 0;

  // CPU
  virtual std::vector<std::string> CpuUtilization() = 0;
  virtual long Jiffies() = 0;
  virtual long ActiveJiffies() = 0;
  virtual long ActiveJiffies(int pid) = 0;
  virtual long IdleJiffies() = 0;

  // Processes
  virtual std::string Command(int pid) = 0;
  virtual std::string Ram(int pid) = 0;
  virtual std::string Uid(int pid) = 0;
  virtual std::string User(int pid) = 0;
  virtual long int UpTime(int pid) = 0;

};

class LinuxParser: public ILinuxParser {
public:
  // System
  float MemoryUtilization() override;
  long UpTime() override;
  std::vector<int> Pids() override;
  int TotalProcesses() override;
  int RunningProcesses() override;
  std::string OperatingSystem() override;
  std::string Kernel() override;

  // CPU
  std::vector<std::string> CpuUtilization() override;
  long Jiffies() override;
  long ActiveJiffies() override;
  long ActiveJiffies(int pid) override;
  long IdleJiffies() override;

  // Processes
  std::string Command(int pid) override;
  std::string Ram(int pid) override;
  std::string Uid(int pid) override;
  std::string User(int pid) override;
  long int UpTime(int pid) override;

private:
  // Paths
  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
  const std::string kCpuinfoFilename{"/cpuinfo"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};
  enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };
};

#endif