#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  cached_process_ticks_ = 0;
  cached_system_ticks_ = 0;
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const { return cpu_; }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const &a) const {
  return CpuUtilization() < a.CpuUtilization() ? true : false;
}

bool Process::operator>(Process const &a) const {
  return CpuUtilization() > a.CpuUtilization() ? true : false;
}

void Process::CpuUtilization(long process_ticks, long system_ticks) {
  long duration_process = process_ticks - cached_process_ticks_;
  long duration = system_ticks - cached_system_ticks_;
  cpu_ = (float)(duration_process) / duration;
  cached_process_ticks_ = process_ticks;
  cached_system_ticks_ = system_ticks;
}