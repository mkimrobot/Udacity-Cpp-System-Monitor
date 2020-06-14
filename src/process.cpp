#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): m_pid(pid) {
    hertz = sysconf(_SC_CLK_TCK);
}

// Return this process's ID
int Process::Pid() { 
    return m_pid; 
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long uptime;
    long utime, stime, cutime, cstime, starttime;
    long total_time;
    float seconds, cpu_usage;

    // uptime of the system
    uptime = LinuxParser::UpTime();

    // PID stat
    auto pid_stat = LinuxParser::CpuUtilization(m_pid);

    utime = std::stol(pid_stat[13]);
    stime = std::stol(pid_stat[14]);
    cutime = std::stol(pid_stat[15]);
    cstime = std::stol(pid_stat[16]);
    starttime = std::stol(pid_stat[21]);

    // Calculation
    total_time = utime + stime + cutime + cstime;
    // total_time = utime + stime;
    seconds = uptime - (starttime / hertz);
    cpu_usage = (total_time / hertz / seconds);

    return cpu_usage;
}

// DONE: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(m_pid); 
}

// DONE: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(m_pid);
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(m_pid);
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long uptime = LinuxParser::UpTime(m_pid);
    return uptime / hertz;    
}

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: a[[maybe_unused]] once you define the function -> DONE
bool Process::operator>(Process &a){ 
    return CpuUtilization() > a.CpuUtilization();
}