#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const ILinuxProcessParser& parser, int pid)
    : pid_(pid), parser_(parser) {}

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
  const float hertz = sysconf(_SC_CLK_TCK);
  const float total_time = (float)parser_.ActiveJiffies(pid_) / hertz;
  const auto uptime = parser_.UpTime();
  const auto starttime = parser_.UpTime(pid_);

  return total_time / (float)(uptime - starttime);
}

// DONE: Return the command that generated this process
string Process::Command() { return parser_.Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return parser_.Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return parser_.User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return parser_.UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }