#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  System(const ILinuxParser& parser);
  IProcessor& Cpu();
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  // TODO: Define any necessary private members
 private:
  const ILinuxParser& parser;
  Processor cpu_;
  std::vector<Process> processes_ = {};
};

#endif