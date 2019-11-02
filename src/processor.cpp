#include "processor.h"

Processor::Processor(const ILinuxProcessorParser& parser) : parser_(parser) {}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return 0.0; }