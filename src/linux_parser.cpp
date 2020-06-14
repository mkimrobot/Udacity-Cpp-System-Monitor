#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  bool memTotalFound=false, memFreeFound=false;
  float memTotal, memFree;
  string key;
  float value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  
  if(stream.is_open()){    
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;

      if(key == "MemTotal:"){
        memTotal = value;  
        memTotalFound = true;      
      }
      else if(key == "MemFree:"){
        memFree = value;    
        memFreeFound = true;    
      }

      // break the loop if all informations are found
      if (memTotalFound && memFreeFound){
        break;
      }
    }
  }

  return (memTotal - memFree) / memTotal;  
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }  
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// MK added function
vector<string> LinuxParser::Utilization(){
  string line, key;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key){
        if (key == "cpu"){
          vector<string> results((std::istream_iterator<string>(linestream)), std::istream_iterator<string>());
          return results;  
        }
      }
    }
  }
  // this will not happen: if no match -> return empty
  return vector<string>{};
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid) { 
  string pid_string = "/" + std::to_string(pid);
  string line;
  std::ifstream stream(kProcDirectory + pid_string + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::istringstream linestream(line);

  vector<string> results((std::istream_iterator<string>(linestream)), std::istream_iterator<string>());
  return results;  
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string pid_string = "/" + std::to_string(pid);
  string cmdline, command;
  std::ifstream stream(kProcDirectory + pid_string + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmdline);
  }
  std::istringstream linestream(cmdline);
  linestream >> command;

  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string pid_string = "/" + std::to_string(pid);
  float vmSize, value;
  string key, line;

  std::ifstream stream(kProcDirectory + pid_string + kStatusFilename);  
  if(stream.is_open()){    
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize:"){
        vmSize = value;  
        break;
      }
    }
  }
  return std::to_string(int(vmSize * 0.001));
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function -> DONE
string LinuxParser::Uid(int pid) { 
  string pid_string = "/" + std::to_string(pid);
  string line, key, value, uid;

  std::ifstream stream(kProcDirectory + pid_string + kStatusFilename);  
  if(stream.is_open()){    
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid:"){
        uid = value;  
        break;
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function -> DONE
string LinuxParser::User(int pid) { 
  string line, stream_user, stream_val, stream_uid;
  string uid = LinuxParser::Uid(pid);
  
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> stream_user >> stream_val >> stream_uid){
        if(stream_uid == uid){
          return stream_user;
        }
      }
    }
  }
  // this will not happen: if no match -> return empty
  return string();
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function -> DONE
long LinuxParser::UpTime(int pid) { 
  auto pid_stat = LinuxParser::CpuUtilization(pid);
  return std::stol(pid_stat[21]);
}