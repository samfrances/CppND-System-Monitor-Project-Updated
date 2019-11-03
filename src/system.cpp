#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(const ILinuxParser& p) : parser_(p), cpu_(Processor(parser_)) {}

// DONE: Return the system's CPU
IProcessor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  std::set<Process> sortedProcs;
  for (auto pid : parser_.Pids()) {
    Process proc(parser_, pid);
    sortedProcs.insert(proc);
  }
  std::copy(sortedProcs.begin(), sortedProcs.end(),
            std::back_inserter(processes_));
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return parser_.Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return parser_.MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return parser_.OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return parser_.RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return parser_.TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return parser_.UpTime(); }