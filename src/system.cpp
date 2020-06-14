#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    // PID list may be diffrent from previous. Empty the container
    processes_ = {};

    std::vector<int> pids = LinuxParser::Pids();

    for(int pid: pids){
        processes_.push_back(Process(pid));
    }

    // for (int pid: pids){
    //     std::cout << "PID: " << pid << ". ";
    // }


    std::sort(processes_.begin(), processes_.end(), [](auto &process1, auto &process2){
        return process1 > process2;
    });


    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long System::UpTime() { 
    return LinuxParser::UpTime();
}