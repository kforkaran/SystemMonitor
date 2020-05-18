#include <cstddef>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  cpu_ = Processor();
  Processes();
}

Processor &System::Cpu() { return cpu_; }

vector<Process> &System::Processes() {
  processes_.clear();
  vector<int> pids = LinuxParser::Pids();
  for (int pid : pids)
    processes_.push_back(Process(pid));
  for (Process &process : processes_)
    process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()),
                           LinuxParser::Jiffies());

  sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }