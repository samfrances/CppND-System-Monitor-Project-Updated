#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  Processor(const ILinuxProcessorParser& parser);
  float Utilization();

 private:
  const ILinuxProcessorParser& parser_;
  bool bootstrapping_ = true;
  long prev_total_jiffies = 0;
  long prev_active_jiffies = 0;
  float prev_result = 0;
};

#endif