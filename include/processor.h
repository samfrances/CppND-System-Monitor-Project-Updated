#ifndef PROCESSOR_H
#define PROCESSOR_H

class IProcessor {
 public:
  virtual float Utilization() = 0;
};

class Processor : public IProcessor {
 public:
  float Utilization() override;  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif