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

};  // namespace LinuxParserPure

#endif