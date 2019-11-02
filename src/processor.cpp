#include "processor.h"

Processor::Processor(const ILinuxProcessorParser& parser) : parser_(parser) {}

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  auto snapshot = parser_.CpuUtilization();
  if (bootstrapping_) {
    prev_total_jiffies = snapshot.Jiffies();
    prev_active_jiffies = snapshot.ActiveJiffies();
    bootstrapping_ = false;
    return 0;
  }

  long new_total = snapshot.Jiffies();
  long new_active = snapshot.ActiveJiffies();

  if (new_total <= prev_total_jiffies || new_active < prev_active_jiffies) {
    return prev_result;
  }

  float result = (float)(new_active - prev_active_jiffies) /
                 (float)(new_total - prev_total_jiffies);

  prev_total_jiffies = new_total;
  prev_active_jiffies = new_active;
  prev_result = result;
  return result;
}