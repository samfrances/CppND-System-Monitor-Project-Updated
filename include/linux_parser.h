#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

class ILinuxParser {
 public:
  // System
  virtual float MemoryUtilization() const = 0;
  virtual long UpTime() const = 0;
  virtual std::vector<int> Pids() const = 0;
  virtual int TotalProcesses() const = 0;
  virtual int RunningProcesses() const = 0;
  virtual std::string OperatingSystem() const = 0;
  virtual std::string Kernel() const = 0;

  // CPU
  virtual std::vector<std::string> CpuUtilization() const = 0;  // TODO consider making private
  virtual long Jiffies() const = 0;
  virtual long ActiveJiffies() const = 0;
  virtual long ActiveJiffies(int pid) const = 0;
  virtual long IdleJiffies() const = 0;

  // Processes
  virtual std::string Command(int pid) const = 0;
  virtual std::string Ram(int pid) const = 0;
  virtual std::string Uid(int pid) const = 0;
  virtual std::string User(int pid) const = 0;
  virtual long int UpTime(int pid) const = 0;
};

class LinuxParser : public ILinuxParser {
 public:
  LinuxParser(std::string prefix = "");
  // System
  float MemoryUtilization() const override;
  long UpTime() const override;
  std::vector<int> Pids() const override;
  int TotalProcesses() const override;
  int RunningProcesses() const override;
  std::string OperatingSystem() const override;
  std::string Kernel() const override;

  // CPU
  std::vector<std::string> CpuUtilization() const override;
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