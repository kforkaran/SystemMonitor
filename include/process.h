#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
*/
class Process {
public:
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const &a) const;
  Process(int pid);

private:
  int pid_;
  float cpu_ = 0;
  long cached_process_ticks_ = 0;
  long cached_system_ticks_ = 0;
};

#endif