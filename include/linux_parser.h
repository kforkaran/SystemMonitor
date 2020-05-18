#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string ProcDirectory{"/proc/"};
const std::string CmdlineFilename{"/cmdline"};
const std::string CpuinfoFilename{"/cpuinfo"};
const std::string StatusFilename{"/status"};
const std::string StatFilename{"/stat"};
const std::string UptimeFilename{"/uptime"};
const std::string MeminfoFilename{"/meminfo"};
const std::string VersionFilename{"/version"};
const std::string OSPath{"/etc/os-release"};
const std::string PasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  User_ = 0,
  Nice_,
  System_,
  Idle_,
  IOwait_,
  IRQ_,
  SoftIRQ_,
  Steal_,
  Guest_,
  GuestNice_
};

std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
}; // namespace LinuxParser

#endif