#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class IProcessor {
 public:
  virtual float Utilization() = 0;
};

class Processor : public IProcessor {
 public:
  Processor(const ILinuxProcessorParser& parser);
  float Utilization() override;  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  const ILinuxProcessorParser& parser_;
};

#endif