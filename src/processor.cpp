#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    // get current state
    auto cpuInfo = LinuxParser::Utilization();

    // Algorithm to calculate CPU usage percentage
    PrevIdle = preCpuInfo[LinuxParser::CPUStates::kIdle_] + preCpuInfo[LinuxParser::CPUStates::kIOwait_];
    Idle = std::stoi(cpuInfo[LinuxParser::CPUStates::kIdle_]) + std::stoi(cpuInfo[LinuxParser::CPUStates::kIOwait_]);

    PrevNonIdle = preCpuInfo[LinuxParser::CPUStates::kUser_] + preCpuInfo[LinuxParser::CPUStates::kNice_] + 
                  preCpuInfo[LinuxParser::CPUStates::kSystem_] + preCpuInfo[LinuxParser::CPUStates::kIRQ_] + 
                  preCpuInfo[LinuxParser::CPUStates::kSoftIRQ_] + preCpuInfo[LinuxParser::CPUStates::kSteal_];
    NonIdle = std::stoi(cpuInfo[LinuxParser::CPUStates::kUser_]) + std::stoi(cpuInfo[LinuxParser::CPUStates::kNice_]) + 
              std::stoi(cpuInfo[LinuxParser::CPUStates::kSystem_]) + std::stoi(cpuInfo[LinuxParser::CPUStates::kIRQ_]) + 
              std::stoi(cpuInfo[LinuxParser::CPUStates::kSoftIRQ_]) + std::stoi(cpuInfo[LinuxParser::CPUStates::kSteal_]);

    PrevTotal = PrevIdle + PrevNonIdle;
    Total = Idle + NonIdle;

    totald = Total - PrevTotal;
    idled = Idle - PrevIdle;

    cpu_percentage = (float)(totald - idled) / (float)totald;

    // update previous states
    preCpuInfo[LinuxParser::CPUStates::kIdle_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kIdle_]);
    preCpuInfo[LinuxParser::CPUStates::kIOwait_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kIOwait_]);
    preCpuInfo[LinuxParser::CPUStates::kUser_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kUser_]);
    preCpuInfo[LinuxParser::CPUStates::kNice_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kNice_]);
    preCpuInfo[LinuxParser::CPUStates::kSystem_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kSystem_]);
    preCpuInfo[LinuxParser::CPUStates::kIRQ_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kIRQ_]);
    preCpuInfo[LinuxParser::CPUStates::kSoftIRQ_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kSoftIRQ_]);
    preCpuInfo[LinuxParser::CPUStates::kSteal_] = std::stoi(cpuInfo[LinuxParser::CPUStates::kSteal_]);

    return cpu_percentage;
}
// cat /proc/stat
// cpu* information: add up number of giffes / total number of giffies