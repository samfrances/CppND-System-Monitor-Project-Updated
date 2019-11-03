#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "linux_parser_pure.h"

class ILinuxProcessorParser {
 public:
  // CPU
  virtual LinuxParserPure::CpuUtilizationSnapshot CpuUtilization() const = 0;
  virtual long Jiffies() const = 0;        // TODO consider deleting
  virtual long ActiveJiffies() const = 0;  // TODO consider deleting
  virtual long IdleJiffies() const = 0;  // TODO consider deleting
};

class ILinuxProcessParser {
 public:
  virtual std::string Command(int pid) const = 0;
  virtual std::string Ram(int pid) const = 0;
  virtual std::string Uid(int pid) const = 0;
  virtual std::string User(int pid) const = 0;
  virtual long int UpTime(int pid) const = 0;
  virtual long ActiveJiffies(int pid) const = 0;
};

class ILinuxParser : public ILinuxProcessorParser, public ILinuxProcessParser {
 public:
  // System
  virtual float MemoryUtilization() const = 0;
  virtual long UpTime() const = 0;
  virtual std::vector<int> Pids() const = 0;
  virtual int TotalProcesses() const = 0;
  virtual int RunningProcesses() const = 0;
  virtual std::string OperatingSystem() const = 0;
  virtual std::string Kernel() const = 0;
};

class LinuxParser : public ILinuxParser {
 public:
  LinuxParser(std::string prefix = "/");
  // System
  float MemoryUtilization() const override;
  long UpTime() const override;
  std::vector<int> Pids() const override;
  int TotalProcesses() const override;
  int RunningProcesses() const override;
  std::string OperatingSystem() const override;
  std::string Kernel() const override;

  // CPU
  LinuxParserPure::CpuUtilizationSnapshot CpuUtilization() const override;
  long Jiffies() const override;
  long ActiveJiffies() const override;
  long ActiveJiffies(int pid) const override;
  long IdleJiffies() const override;

  // Processes
  std::string Command(int pid) const override;
  std::string Ram(int pid) const override;
  std::string Uid(int pid) const override;
  std::string User(int pid) const override;
  long int UpTime(int pid) const override;

 private:
  // Paths
  const std::string kRoot;

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