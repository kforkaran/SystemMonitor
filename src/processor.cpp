#include "processor.h"
#include "linux_parser.h"

Processor::Processor() {
  tAll_ = LinuxParser::Jiffies();
  tIdle_ = LinuxParser::IdleJiffies();
}

float Processor::Utilization() {
  float tAllOld = tAll_, tIdleOld = tIdle_;
  tAll_ = LinuxParser::Jiffies();
  tIdle_ = LinuxParser::IdleJiffies();
  float rValue =
      (((tAll_ + tAllOld) - (tIdle_ + tIdleOld)) / (tAll_ + tAllOld));
  return (rValue > 0.0) ? rValue : 0.0;
}