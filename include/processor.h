#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  Processor();
  float Utilization();

private:
  long tAll_ = 0;
  long tIdle_ = 0;
};

#endif