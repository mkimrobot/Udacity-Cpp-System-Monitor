#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO->DONE: Declare any necessary private members
 private:
    float cpu_percentage;
    int preCpuInfo[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total;
    int totald, idled;
};

#endif