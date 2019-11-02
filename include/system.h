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
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();
  long UpTime();           // TODO: See src/system.cpp
  int TotalProcesses();    // TODO: See src/system.cpp
  int RunningProcesses();  // TODO: See src/system.cpp
  std::string Kernel();
  std::string OperatingSystem();

  // TODO: Define any necessary private members
 private:
  const ILinuxParser& parser;
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif